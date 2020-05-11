#include "interfaces/Controller.h"
#include "interfaces/Notifier.h"
#include "interfaces/View.h"
#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static pthread_mutex_t controller_mutex;

static pthread_rwlock_t commandMap_mutex;

// ControllerMap

typedef struct ControllerMap ControllerMap;

struct ControllerMap {
    const char *name;
    Controller *controller;
    ControllerMap *next;
};

static ControllerMap *instanceMap;

static ControllerMap *NewControllerMap(const char *key, Controller *controller) {
    ControllerMap *self = malloc(sizeof(ControllerMap));
    if (self == NULL) goto exception;
    self->name = key;
    self->controller = controller;
    self->next = NULL;
    return self;

    exception:
        fprintf(stderr, "ControllerMap allocation failed.\n");
        return NULL;
}

static void AddControllerMap(const char *key, Controller *controller) {
    ControllerMap **controllerMap = &instanceMap;
    while (*controllerMap)
        controllerMap = &(*controllerMap)->next;
    *controllerMap = NewControllerMap(key, controller);
}

static Controller *GetControllerMap(const char *key) {
    ControllerMap *controllerMap = instanceMap;
    while (controllerMap && strcmp(controllerMap->name, key) != 0)
        controllerMap = controllerMap->next;
    return controllerMap == NULL ? NULL : controllerMap->controller;
}

static void DeleteControllerMap(ControllerMap *self) {
    free(self->controller);
    free(self);
    self = NULL;
}

static void RemoveControllerMap(const char *key) {
    ControllerMap **controllerMap = &instanceMap;
    while (*controllerMap) {
        if (strcmp((*controllerMap)->name, key) == 0) {
            ControllerMap *node = *controllerMap;
            *controllerMap = (*controllerMap)->next;
            DeleteControllerMap(node);
            break;
        }
        controllerMap = &(*controllerMap)->next;
    }
}

// CommandMap

static CommandMap *NewCommandMap(const char *notificationName, SimpleCommand *(*factory)()) {
    CommandMap *self = malloc(sizeof(CommandMap));
    self->name = notificationName;
    self->factory = factory;
    self->next = NULL;
    return self;
}

static void DeleteCommandMap(CommandMap *self) {
    free(self);
    self = NULL;
}

// Controller

static void initializeController(Controller *self) {
    self->view = getViewInstance(self->multitonKey, NewView);
}

static void executeCommand(Controller *self, Notification *notification) {
    pthread_rwlock_rdlock(&commandMap_mutex);
    CommandMap *cursor = self->commandMap;
    while(cursor && strcmp(cursor->name, notification->getName(notification)) != 0)
        cursor = cursor->next;

    if (cursor) {
        SimpleCommand *commandInstance = cursor->factory();
        commandInstance->notifier->initializeNotifier(commandInstance->notifier, self->multitonKey);
        commandInstance->execute(commandInstance, notification);
        DeleteSimpleCommand(commandInstance);
    }
    pthread_rwlock_unlock(&commandMap_mutex);
}

static void registerCommand(Controller *self, const char *notificationName, SimpleCommand *(factory)()) {
    pthread_rwlock_wrlock(&commandMap_mutex);
    CommandMap **cursor = &self->commandMap;

    while (*cursor) {
        if (strcmp((*cursor)->name, notificationName) == 0) {
            CommandMap *node = NewCommandMap(notificationName, factory);
            node->next = (*cursor)->next;
            DeleteCommandMap(*cursor);
            *cursor = node;
            return;
        }
        cursor = &(*cursor)->next;
    }

    *cursor = NewCommandMap(notificationName, factory);
    Observer *observer = NewObserver((void (*)(void *, Notification *)) self->executeCommand, self);
    self->view->registerObserver(self->view, notificationName, observer);
    pthread_rwlock_unlock(&commandMap_mutex);
}

static bool hasCommand(Controller *self, const char *notificationName) {
    pthread_rwlock_rdlock(&commandMap_mutex);
    CommandMap *cursor = self->commandMap;
    while(cursor != NULL && strcmp(cursor->name, notificationName) != 0)
        cursor = cursor->next;
    pthread_rwlock_unlock(&commandMap_mutex);
    return cursor != NULL;
}

static void removeCommand(Controller *self, const char *notificationName) {
    pthread_rwlock_wrlock(&commandMap_mutex);
    CommandMap **cursor = &self->commandMap;

    while (*cursor) {
        if (strcmp((*cursor)->name, notificationName) == 0) {
            CommandMap *commandMap = (*cursor);
            *cursor = (*cursor)->next;
            self->view->removeObserver(self->view, notificationName, self);
            DeleteCommandMap(commandMap);
            break;
        }
        cursor = &(*cursor)->next;
    }
    pthread_rwlock_unlock(&commandMap_mutex);
}

void InitController(Controller *self) {
    self->commandMap = NULL;
    self->initializeController = initializeController;
    self->executeCommand = executeCommand;
    self->registerCommand = registerCommand;
    self->hasCommand = hasCommand;
    self->removeCommand = removeCommand;
}

Controller *NewController(const char *key) {
    assert(GetControllerMap(key) == NULL);
    Controller *self = malloc(sizeof(Controller));
    if (self == NULL) goto exception;
    InitController(self);
    self->multitonKey = key;
    return self;

    exception:
        fprintf(stderr, "Controller allocation failed.\n");
        return NULL;
}

void DeleteController(const char *key) {
    pthread_mutex_lock(&controller_mutex);
    RemoveControllerMap(key);
    pthread_mutex_unlock(&controller_mutex);
}

Controller *getControllerInstance(const char *key, Controller *(*factory)(const char *)) {
    pthread_mutex_lock(&controller_mutex);
    Controller *instance = GetControllerMap(key);
    if (instance == NULL) {
        instance = factory(key);
        AddControllerMap(key, instance);
        instance->initializeController(instance);
    }
    pthread_mutex_unlock(&controller_mutex);
    return instance;
}