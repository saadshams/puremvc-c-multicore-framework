/**
* @file Controller.c
* @internal
* @brief Controller Implementation
*
* @author Saad Shams <saad.shams@puremvc.org>
* @copyright BSD 3-Clause License
*/
#include <stdlib.h>
#include <string.h>
#include <collection/IDictionary.h>

#include "Controller.h"
#include "View.h"

// The Multiton Controller instanceMap.
static struct IDictionary *instanceMap = NULL;

// mutex for controller instanceMap
static MutexOnce token = MUTEX_ONCE_INIT;
static Mutex mutex;

static void initializeController(struct IController *self, const char **error) {
    struct Controller *this = (struct Controller *) self;
    this->view = puremvc_view_getInstance(this->multitonKey, puremvc_view_new, error);
}

static void executeCommand(const struct IController *self, struct INotification *notification, const char **error) {
    struct Controller *this = (struct Controller *) self;

    mutex_lock_shared(&this->commandMapMutex);
    struct ICommand *(*factory)(const char **error) = (struct ICommand *(*)(const char **)) this->commandMap->get(this->commandMap, notification->getName(notification));
    mutex_unlock(&this->commandMapMutex);

    if (factory != NULL) {
        struct ICommand *command = factory(error);
        if (*error != NULL) return;

        command->notifier->initializeNotifier(command->notifier, this->multitonKey, error);
        if (*error != NULL) return puremvc_simple_command_free(&command);

        command->execute(command, notification, error);
        if (*error != NULL) return puremvc_simple_command_free(&command);

        puremvc_simple_command_free(&command);
    }
}

static void registerCommand(const struct IController *self, const char *notificationName, struct ICommand *(*factory)(const char **error), const char **error) {
    struct Controller *this = (struct Controller *) self;

    mutex_lock(&this->commandMapMutex);
    if (this->commandMap->containsKey(this->commandMap, notificationName) == false) {
        const struct IObserver *observer = puremvc_observer_new((const void (*)(const void *, struct INotification *))executeCommand, this, error);
        if (*error != NULL) return mutex_unlock(&this->commandMapMutex), (void)0;

        this->view->registerObserver(this->view, notificationName, observer, error);
        if (*error != NULL) return mutex_unlock(&this->commandMapMutex), (void)0;

        this->commandMap->put(this->commandMap, notificationName, factory, error);
        if (*error != NULL) return this->view->removeObserver(this->view, notificationName, this), mutex_unlock(&this->commandMapMutex), (void)0;
    } else {
        this->commandMap->replace(this->commandMap, notificationName, factory);
    }
    mutex_unlock(&this->commandMapMutex);
}

static bool hasCommand(const struct IController *self, const char *notificationName) {
    struct Controller *this = (struct Controller *) self;
    mutex_lock_shared(&this->commandMapMutex);
    const bool exists = this->commandMap->containsKey(this->commandMap, notificationName);
    mutex_unlock(&this->commandMapMutex);
    return exists;
}

static void removeCommand(const struct IController *self, const char *notificationName) {
    struct Controller *this = (struct Controller *) self;

    mutex_lock(&this->commandMapMutex);
    if (this->commandMap->containsKey(this->commandMap, notificationName)) {
        this->commandMap->removeItem(this->commandMap, notificationName);
        this->view->removeObserver(this->view, notificationName, this);
    }
    mutex_unlock(&this->commandMapMutex);
}

static struct Controller *init(struct Controller *controller) {
    if (controller == NULL) return NULL;
    controller->base.initializeController = initializeController;
    controller->base.executeCommand = executeCommand;
    controller->base.registerCommand = registerCommand;
    controller->base.hasCommand = hasCommand;
    controller->base.removeCommand = removeCommand;
    return controller;
}

static struct Controller *alloc(const char *key, const char **error) {
    struct Controller *controller = malloc(sizeof(struct Controller));
    if (controller == NULL) return *error = "[PureMVC::Controller::alloc] Error: Failed to allocate Controller", NULL;

    memset(controller, 0, sizeof(struct Controller));

    controller->multitonKey = strdup(key);
    if (controller->multitonKey == NULL) return *error = "[PureMVC::Controller::alloc] Error: Failed to allocate Controller key (strdup)", free(controller), NULL;

    mutex_init(&controller->commandMapMutex);

    controller->commandMap = collection_dictionary_new(error);
    if (*error != NULL) return mutex_destroy(&controller->commandMapMutex), free((void *)controller->multitonKey), free(controller), NULL;

    return controller;
}

struct IController *puremvc_controller_new(const char *key, const char **error) {
    if (key == NULL) return *error = "[PureMVC::Controller::new] Error: key must not be NULL.", NULL;
    return (struct IController *) init(alloc(key, error));
}

static void command_free(void *value) {
    struct ICommand *command = value;
    puremvc_simple_command_free(&command);
}

void puremvc_controller_free(struct IController **controller) {
    if (controller == NULL || *controller == NULL) return;
    struct Controller *this = (struct Controller *) *controller;

    free((void *) this->multitonKey);
    this->commandMap->clear(this->commandMap, command_free);
    collection_dictionary_free(&this->commandMap);

    mutex_destroy(&this->commandMapMutex);

    free(this);
    *controller = NULL;
}

static void dispatchOnce() {
    mutex_init(&mutex);
}

struct IController *puremvc_controller_getInstance(const char *key, struct IController *(*factory)(const char *key, const char **error), const char **error) {
    if (key == NULL || factory == NULL) return *error = "[PureMVC::Controller::getInstance] Error: key or factory must not be NULL.", NULL;
    mutex_once(&token, dispatchOnce);
    mutex_lock(&mutex);

    if (instanceMap == NULL) {
        instanceMap = collection_dictionary_new(error);
        if (*error != NULL) return mutex_unlock(&mutex), NULL;
    }

    struct IController *instance = (struct IController *) instanceMap->get(instanceMap, key);
    if (instance == NULL) {
        instance = factory(key, error);
        if (*error != NULL) return mutex_unlock(&mutex), NULL;

        instance->initializeController(instance, error);
        if (*error != NULL) return puremvc_controller_free(&instance), mutex_unlock(&mutex), NULL;

        instanceMap->put(instanceMap, key, instance, error);
        if (*error != NULL) return puremvc_controller_free(&instance), mutex_unlock(&mutex), NULL;
    }

    mutex_unlock(&mutex);
    return instance;
}

void puremvc_controller_removeController(const char *key) {
    if (key == NULL) return;
    mutex_lock(&mutex);

    struct IController *controller = instanceMap->removeItem(instanceMap, key);
    if (controller != NULL) puremvc_controller_free(&controller);

    mutex_unlock(&mutex);
}
