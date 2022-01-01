#include "interfaces/Controller.h"
#include "interfaces/Notifier.h"
#include "interfaces/View.h"
#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct ControllerNode ControllerNode;

// ControllerNode LinkedList
struct ControllerNode {
    const char *name;
    Controller *controller;
    ControllerNode *next;
};

// The Multiton Controller instanceMap.
static ControllerNode *instanceMap;

// mutex for controller instanceMap
static pthread_rwlock_t controller_mutex;

/** Construct a new instanceMap node */
static ControllerNode *NewControllerNode(const char *key, Controller *controller) {
    ControllerNode *node = malloc(sizeof(ControllerNode));
    if (node == NULL) goto exception;
    node->name = key;
    node->controller = controller;
    node->next = NULL;
    return node;

    exception:
        fprintf(stderr, "ControllerNode allocation failed.\n");
        return NULL;
}

/** Retrieve a Node from instanceMap LinkedList */
static Controller *GetControllerNode(const char *key) {
    ControllerNode *cursor = instanceMap;
    while (cursor && strcmp(cursor->name, key) != 0)
        cursor = cursor->next;
    return cursor != NULL ? cursor->controller : NULL;
}

/** Add a Node to the instanceMap LinkedList */
static void AddControllerNode(const char *key, Controller *controller) {
    ControllerNode **cursor = &instanceMap;
    while (*cursor)
        cursor = &(*cursor)->next;
    *cursor = NewControllerNode(key, controller);
}

/** Remove a node from instanceMap LinkedList */
static void RemoveControllerNode(const char *key) {
    ControllerNode **cursor = &instanceMap;
    while (*cursor) {
        if (strcmp((*cursor)->name, key) == 0) {
            ControllerNode *node = *cursor;
            *cursor = (*cursor)->next;
            free(node->controller);
            free(node);
            node = NULL;
            break;
        }
        cursor = &(*cursor)->next;
    }
}

struct CommandNode {
    const char *name;
    SimpleCommand *(*factory)(void);
    CommandNode *next;
};

// mutex for commandMap
static pthread_rwlock_t commandMap_mutex = PTHREAD_MUTEX_INITIALIZER;

/** Construct a new commandMap node */
static CommandNode *NewCommandNode(const char *notificationName, SimpleCommand *(*factory)(void)) {
    CommandNode *self = malloc(sizeof(CommandNode));
    self->name = notificationName;
    self->factory = factory;
    self->next = NULL;
    return self;
}

/** Release the memory for a commandMap node */
static void DeleteCommandNode(CommandNode *self) {
    free(self);
    self = NULL;
}

static void initializeController(Controller *self) {
    self->view = $View.getInstance(self->multitonKey, $View.new);
}

static void executeCommand(Controller *self, Notification *notification) {
    pthread_rwlock_rdlock(&commandMap_mutex);
    CommandNode *cursor = self->commandMap;
    while(cursor && strcmp(cursor->name, notification->getName(notification)) != 0)
        cursor = cursor->next;

    if (cursor) {
        SimpleCommand *commandInstance = cursor->factory();
        commandInstance->notifier->initializeNotifier(commandInstance->notifier, self->multitonKey);
        commandInstance->execute(commandInstance, notification);
        $SimpleCommand.delete(commandInstance);
    }
    pthread_rwlock_unlock(&commandMap_mutex);
}

static void registerCommand(Controller *self, const char *notificationName, SimpleCommand *(factory)(void)) {
    pthread_rwlock_wrlock(&commandMap_mutex);
    CommandNode **cursor = &self->commandMap;

    while (*cursor) {
        if (strcmp((*cursor)->name, notificationName) == 0) {
            CommandNode *node = NewCommandNode(notificationName, factory);
            node->next = (*cursor)->next;
            DeleteCommandNode(*cursor);
            *cursor = node;
            return;
        }
        cursor = &(*cursor)->next;
    }

    *cursor = NewCommandNode(notificationName, factory);
    Observer *observer = $Observer.new((void (*)(void *, Notification *)) self->executeCommand, self);
    self->view->registerObserver(self->view, notificationName, observer);
    pthread_rwlock_unlock(&commandMap_mutex);
}

static bool hasCommand(Controller *self, const char *notificationName) {
    pthread_rwlock_rdlock(&commandMap_mutex);
    CommandNode *cursor = self->commandMap;
    while(cursor != NULL && strcmp(cursor->name, notificationName) != 0)
        cursor = cursor->next;
    pthread_rwlock_unlock(&commandMap_mutex);
    return cursor != NULL;
}

static void removeCommand(Controller *self, const char *notificationName) {
    pthread_rwlock_wrlock(&commandMap_mutex);
    CommandNode **cursor = &self->commandMap;

    while (*cursor) {
        // if the Command is registered...
        if (strcmp((*cursor)->name, notificationName) == 0) {
            CommandNode *commandMap = (*cursor);

            // remove the command
            *cursor = (*cursor)->next;

            // remove the observer
            self->view->removeObserver(self->view, notificationName, self);
            DeleteCommandNode(commandMap);
            break;
        }
        cursor = &(*cursor)->next;
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
    assert(GetControllerNode(key) == NULL);

    Controller *controller = malloc(sizeof(Controller));
    if (controller == NULL) goto exception;
    init(controller);
    controller->multitonKey = key;
    AddControllerNode(key, controller);
    return controller;

    exception:
        fprintf(stderr, "Controller allocation failed.\n");
        return NULL;
}

static void removeController(const char *key) {
    pthread_rwlock_wrlock(&controller_mutex);
    RemoveControllerNode(key);
    pthread_rwlock_unlock(&controller_mutex);
}

static Controller *getInstance(const char *key, Controller *(*factory)(const char *)) {
    pthread_rwlock_wrlock(&controller_mutex);
    Controller *instance = GetControllerNode(key);
    if (instance == NULL) {
        instance = factory(key);
        instance->initializeController(instance);
    }
    pthread_rwlock_unlock(&controller_mutex);
    return instance;
}

const struct $Controller $Controller = { .new = new, .init = init, .removeController = removeController, .getInstance = getInstance };
