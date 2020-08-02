#include "interfaces/Controller.h"
#include "interfaces/Notifier.h"
#include "interfaces/View.h"
#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct ControllerMap ControllerMap;

// ControllerMap LinkedList
struct ControllerMap {
    const char *name;
    Controller *controller;
    ControllerMap *next;
};

// The Multiton Controller instanceMap.
static ControllerMap *instanceMap;

// mutex for controller instanceMap
static pthread_rwlock_t controller_mutex;

/** Construct a new instanceMap node */
static ControllerMap *NewControllerMap(const char *key, Controller *controller) {
    ControllerMap *controllerMap = malloc(sizeof(ControllerMap));
    if (controllerMap == NULL) goto exception;
    controllerMap->name = key;
    controllerMap->controller = controller;
    controllerMap->next = NULL;
    return controllerMap;

    exception:
        fprintf(stderr, "ControllerMap allocation failed.\n");
        return NULL;
}

/** Retrieve a Node from instanceMap LinkedList */
static Controller *GetControllerMap(const char *key) {
    ControllerMap *controllerMap = instanceMap;
    while (controllerMap && strcmp(controllerMap->name, key) != 0)
        controllerMap = controllerMap->next;
    return controllerMap == NULL ? NULL : controllerMap->controller;
}

/** Add a Node to the instanceMap LinkedList */
static void AddControllerMap(const char *key, Controller *controller) {
    ControllerMap **controllerMap = &instanceMap;
    while (*controllerMap)
        controllerMap = &(*controllerMap)->next;
    *controllerMap = NewControllerMap(key, controller);
}

/** Remove a node from instanceMap LinkedList */
static void RemoveControllerMap(const char *key) {
    ControllerMap **controllerMap = &instanceMap;
    while (*controllerMap) {
        if (strcmp((*controllerMap)->name, key) == 0) {
            ControllerMap *node = *controllerMap;
            *controllerMap = (*controllerMap)->next;
            free(node->controller);
            free(node);
            node = NULL;
            break;
        }
        controllerMap = &(*controllerMap)->next;
    }
}

struct CommandMap {
    const char *name;
    SimpleCommand *(*factory)(void);
    CommandMap *next;
};

// mutex for commandMap
static pthread_rwlock_t commandMap_mutex = PTHREAD_MUTEX_INITIALIZER;

/** Construct a new commandMap node */
static CommandMap *NewCommandMap(const char *notificationName, SimpleCommand *(*factory)(void)) {
    CommandMap *self = malloc(sizeof(CommandMap));
    self->name = notificationName;
    self->factory = factory;
    self->next = NULL;
    return self;
}

/** Release the memory for a commandMap node */
static void DeleteCommandMap(CommandMap *self) {
    free(self);
    self = NULL;
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

static void registerCommand(Controller *self, const char *notificationName, SimpleCommand *(factory)(void)) {
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
        // if the Command is registered...
        if (strcmp((*cursor)->name, notificationName) == 0) {
            CommandMap *commandMap = (*cursor);

            // remove the command
            *cursor = (*cursor)->next;

            // remove the observer
            self->view->removeObserver(self->view, notificationName, self);
            DeleteCommandMap(commandMap);
            break;
        }
        cursor = &(*cursor)->next;
    }
    pthread_rwlock_unlock(&commandMap_mutex);
}

void InitController(Controller *controller) {
    controller->commandMap = NULL;
    controller->initializeController = initializeController;
    controller->executeCommand = executeCommand;
    controller->registerCommand = registerCommand;
    controller->hasCommand = hasCommand;
    controller->removeCommand = removeCommand;
}

Controller *NewController(const char *key) {
    assert(GetControllerMap(key) == NULL);

    Controller *controller = malloc(sizeof(Controller));
    if (controller == NULL) goto exception;
    InitController(controller);
    controller->multitonKey = key;
    AddControllerMap(key, controller);
    return controller;

    exception:
        fprintf(stderr, "Controller allocation failed.\n");
        return NULL;
}

void RemoveController(const char *key) {
    pthread_rwlock_wrlock(&controller_mutex);
    RemoveControllerMap(key);
    pthread_rwlock_unlock(&controller_mutex);
}

Controller *getControllerInstance(const char *key, Controller *(*factory)(const char *)) {
    pthread_rwlock_wrlock(&controller_mutex);
    Controller *instance = GetControllerMap(key);
    if (instance == NULL) {
        instance = factory(key);
        instance->initializeController(instance);
    }
    pthread_rwlock_unlock(&controller_mutex);
    return instance;
}
