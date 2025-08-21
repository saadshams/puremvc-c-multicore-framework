#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <collection/IDictionary.h>

#include "Controller.h"
#include "View.h"

// The Multiton Controller instanceMap.
static struct IDictionary *instanceMap;

// mutex for controller instanceMap
static MutexOnce token = MUTEX_ONCE_INIT;
static Mutex mutex;

static void initializeController(struct IController *self) {
    struct Controller *this = (struct Controller *) self;
    this->view = puremvc_view_getInstance(this->multitonKey, puremvc_view_new);
}

static void executeCommand(const struct IController *self, struct INotification *notification) {
    struct Controller *this = (struct Controller *) self;
    mutex_lock_shared(&this->commandMapMutex);

    struct ICommand *(*factory)() = (struct ICommand *(*)()) this->commandMap->get(this->commandMap, notification->getName(notification));
    if (factory != NULL) {
        struct ICommand *command = factory();
        command->notifier->initializeNotifier(command->notifier, this->multitonKey);
        command->execute(command, notification);
        puremvc_simple_command_free(&command);
    }

    mutex_unlock(&this->commandMapMutex);
}

static void registerCommand(const struct IController *self, const char *notificationName, struct ICommand *(factory)(void)) {
    struct Controller *this = (struct Controller *) self;
    mutex_lock(&this->commandMapMutex);

    if (this->commandMap->containsKey(this->commandMap, notificationName) == false) {
        const struct IObserver *observer = puremvc_observer_new((const void (*)(const void *, struct INotification *))executeCommand, this);
        this->view->registerObserver(this->view, notificationName, observer);
        this->commandMap->put(this->commandMap, notificationName, factory);
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

static struct Controller *alloc(const char *key) {
    assert(key != NULL);
    assert(instanceMap->get(instanceMap, key) == NULL);

    struct Controller *controller = malloc(sizeof(struct Controller));
    if (controller == NULL) {
        fprintf(stderr, "[PureMVC::Controller::%s] Error: Failed to allocate Controller with key '%s'.\n", __func__, key);
        return NULL;
    }
    memset(controller, 0, sizeof(struct Controller));

    controller->multitonKey = strdup(key);
    if (controller->multitonKey == NULL) {
        fprintf(stderr, "[PureMVC::Controller::%s] Error: strdup failed for key '%s'.\n", __func__, key);
        free(controller);
        return NULL;
    }

    mutex_init(&controller->commandMapMutex);
    controller->commandMap = collection_dictionary_new();
    return controller;
}

struct IController *puremvc_controller_new(const char *key) {
    return (struct IController *) init(alloc(key));
}

void puremvc_controller_free(struct IController **controller) {
    if (controller == NULL || *controller == NULL) return;
    struct Controller *this = (struct Controller *) *controller;

    free((void *) this->multitonKey);
    this->commandMap->clear(this->commandMap, free);
    collection_dictionary_free(&this->commandMap);
    mutex_destroy(&this->commandMapMutex);

    free(this);
    *controller = NULL;
}

static void dispatchOnce() {
    mutex_init(&mutex);
}

struct IController *puremvc_controller_getInstance(const char *key, struct IController *(*factory)(const char *)) {
    mutex_once(&token, dispatchOnce);
    mutex_lock(&mutex);

    if (instanceMap == NULL) instanceMap = collection_dictionary_new();

    struct IController *instance = (struct IController *) instanceMap->get(instanceMap, key);
    if (instance == NULL) {
        instance = factory(key);
        instanceMap->put(instanceMap, key, instance);
        instance->initializeController(instance);
    }

    mutex_unlock(&mutex);
    return instance;
}

void puremvc_controller_removeController(const char *key) {
    mutex_lock(&mutex);

    struct IController *controller = instanceMap->removeItem(instanceMap, key);
    if (controller != NULL) puremvc_controller_free(&controller);

    mutex_unlock(&mutex);
}
