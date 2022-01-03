#include "interfaces/Controller.h"
#include "interfaces/Notifier.h"
#include "interfaces/View.h"
#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// The Multiton Controller instanceMap.
static Map *instanceMap;

// mutex for controller instanceMap
static pthread_rwlock_t controller_mutex;

// mutex for commandMap
static pthread_rwlock_t commandMap_mutex = PTHREAD_MUTEX_INITIALIZER;

static void initializeController(Controller *self) {
    self->view = $View.getInstance(self->multitonKey, $View.new);
}

static void executeCommand(Controller *self, Notification *notification) {
    pthread_rwlock_rdlock(&commandMap_mutex);

    SimpleCommand *(*factory)(void) = $Map.get(&self->commandMap, notification->name);
    if (factory) {
        SimpleCommand *commandInstance = factory();
        commandInstance->notifier->initializeNotifier(commandInstance->notifier, self->multitonKey);
        commandInstance->execute(commandInstance, notification);
        $SimpleCommand.delete(commandInstance);
    }

    pthread_rwlock_unlock(&commandMap_mutex);
}

static void registerCommand(Controller *self, const char *notificationName, SimpleCommand *(factory)(void)) {
    pthread_rwlock_wrlock(&commandMap_mutex);
    if ($Map.containsKey(&self->commandMap, notificationName) == false) {
        Observer *observer = $Observer.new((void (*)(void *, Notification *)) self->executeCommand, self);
        self->view->registerObserver(self->view, notificationName, observer);

        $Map.put(&self->commandMap, notificationName, factory);
    } else {
        $Map.replace(&self->commandMap, notificationName, factory);
    }
    pthread_rwlock_unlock(&commandMap_mutex);
}

static bool hasCommand(Controller *self, const char *notificationName) {
    pthread_rwlock_rdlock(&commandMap_mutex);
    bool exists = $Map.containsKey(&self->commandMap, notificationName);
    pthread_rwlock_unlock(&commandMap_mutex);
    return exists;
}

static void removeCommand(Controller *self, const char *notificationName) {
    pthread_rwlock_wrlock(&commandMap_mutex);
    if ($Map.containsKey(&self->commandMap, notificationName)) {
        $Map.remove(&self->commandMap, notificationName);

        // remove the observer
        self->view->removeObserver(self->view, notificationName, self);
    }
    pthread_rwlock_unlock(&commandMap_mutex);
}

static void init(Controller *controller) {
    controller->commandMap = NULL;
    controller->initializeController = initializeController;
    controller->executeCommand = executeCommand;
    controller->registerCommand = registerCommand;
    controller->hasCommand = hasCommand;
    controller->removeCommand = removeCommand;
}

static Controller *new(const char *key) {
    assert($Map.get(&instanceMap, key) == NULL);

    Controller *controller = malloc(sizeof(Controller));
    if (controller == NULL) goto exception;
    init(controller);
    controller->multitonKey = key;
    return controller;

    exception:
    fprintf(stderr, "Controller allocation failed.\n");
    return NULL;
}

static void removeController(const char *key) {
    pthread_rwlock_wrlock(&controller_mutex);
    Controller *controller = (Controller *) $Map.remove(&instanceMap, key);
    if (controller != NULL) free(controller);
    pthread_rwlock_unlock(&controller_mutex);
}

static Controller *getInstance(const char *key, Controller *(*factory)(const char *)) {
    pthread_rwlock_wrlock(&controller_mutex);
    Controller *instance = (Controller *) $Map.get(&instanceMap, key);
    if (instance == NULL) {
        instance = (Controller *) $Map.put(&instanceMap, key, factory(key));
        instance->initializeController(instance);
    }
    pthread_rwlock_unlock(&controller_mutex);
    return instance;
}

const struct $Controller $Controller = {.new = new, .init = init, .removeController = removeController, .getInstance = getInstance};
