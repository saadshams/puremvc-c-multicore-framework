#include "interfaces/Controller.h"
#include "interfaces/Notifier.h"
#include "interfaces/View.h"
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>
#include <string.h>

static pthread_mutex_t controller_mutex;

static pthread_rwlock_t commandMap_mutex;

typedef struct ControllerMap ControllerMap;

struct ControllerMap {
    char *name;
    Controller *controller;
    ControllerMap *next;
};

static ControllerMap *instanceMap;

static ControllerMap *NewControllerMap(char *key, Controller *controller) {
    ControllerMap *self = malloc(sizeof(ControllerMap));
    self->name = strdup(key);
    self->controller = controller;
    self->next = NULL;
    return self;
}

static void AddControllerMap(char *key, Controller *controller) {
    ControllerMap **cursor = &instanceMap;
    while (*cursor)
        cursor = &(*cursor)->next;

    *cursor = NewControllerMap(key, controller);
}

static Controller *GetControllerMap(char *key) {
    ControllerMap *cursor = instanceMap;
    while (cursor && strcmp(cursor->name, key) != 0)
        cursor = cursor->next;
    return cursor == NULL ? NULL : cursor->controller;
}

static void RemoveControllerMap(char *key) {
    ControllerMap **cursor = &instanceMap;
    while (*cursor) {
        if (strcmp((*cursor)->name, key) == 0) {
            *cursor = (*cursor)->next;
            free((*cursor)->controller);
            free(*cursor);
            break;
        }
        cursor = &(*cursor)->next;
    }
}

static CommandMap *NewCommandMap(const char *notificationName, SimpleCommand *(*factory)()) {
    CommandMap *self = malloc(sizeof(CommandMap));
    self->name = strdup(notificationName);
    self->factory = factory;
    self->next = NULL;
    return self;
}

static void DeleteCommandMap(CommandMap *self) {
    free(self->name);
    self->factory = NULL;
    self->next = NULL;
    free(self);
}

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
            DeleteCommandMap(commandMap);
            break;
        }
        cursor = &(*cursor)->next;
    }
    pthread_rwlock_unlock(&commandMap_mutex);
}

void InitController(Controller *self) {
    if (self) {
        self->commandMap = NULL;
        self->initializeController = initializeController;
        self->executeCommand = executeCommand;
        self->registerCommand = registerCommand;
        self->hasCommand = hasCommand;
        self->removeCommand = removeCommand;
    }
}

Controller *NewController(char *key) {
    assert(GetControllerMap(key) == NULL);
    Controller *self = malloc(sizeof(Controller));
    InitController(self);
    self->multitonKey = strdup(key);
    return self;
}

void DeleteController(char *key) {
    pthread_mutex_lock(&controller_mutex);
    RemoveControllerMap(key);
    pthread_mutex_unlock(&controller_mutex);
}

Controller *getControllerInstance(char *key, Controller *(*factory)(char *)) {
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