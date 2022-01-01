#include "interfaces/Facade.h"
#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>

typedef struct FacadeNode FacadeNode;

// FacadeNode LinkedList
struct FacadeNode {
    const char *name;
    Facade *facade;
    FacadeNode *next;
};

// The Multiton Facade instanceMap.
static FacadeNode *instanceMap;

// mutex for instanceMap
static pthread_rwlock_t facade_mutex = PTHREAD_MUTEX_INITIALIZER;

/** Construct a new instanceMap node */
static FacadeNode *NewFacadeMap(const char *key, Facade *facade) {
    FacadeNode *node = malloc(sizeof(FacadeNode));
    if (node == NULL) goto exception;
    node->name = key;
    node->facade = facade;
    node->next = NULL;
    return node;

    exception:
        fprintf(stderr, "FacadeNode allocation failed.\n");
        return NULL;
}

/** Retrieve a Node from instanceMap LinkedList */
static Facade *GetFacadeMap(const char *key) {
    FacadeNode *cursor = instanceMap;
    while (cursor && strcmp(cursor->name, key) != 0)
        cursor = cursor->next;
    return cursor != NULL ? cursor->facade : NULL;
}

/** Add a Node to the instanceMap LinkedList */
static void AddFacadeMap(const char *key, Facade *facade) {
    FacadeNode **cursor = &instanceMap;
    while (*cursor)
        cursor = &(*cursor)->next;
    *cursor = NewFacadeMap(key, facade);
}

/** Remove a node from instanceMap LinkedList */
static void RemoveFacadeMap(const char *key) {
    FacadeNode **cursor = &instanceMap;
    while (*cursor) {
        if (strcmp((*cursor)->name, key) == 0) {
            FacadeNode *node = *cursor;
            *cursor = (*cursor)->next;
            free(node->facade);
            free(node);
            node = NULL;
            break;
        }
        cursor = &(*cursor)->next;
    }
}

static void initializeFacade(Facade *self) {
    self->initializeModel(self);
    self->initializeController(self);
    self->initializeView(self);
}

static void initializeController(Facade *self) {
    if (self->controller != NULL) return;
    self->controller = $Controller.getInstance(self->multitonKey, $Controller.new);
}

static void initializeModel(Facade *self) {
    if (self->model != NULL) return;
    self->model = $Model.getInstance(self->multitonKey, $Model.new);
}

static void initializeView(Facade *self) {
    if (self->view != NULL) return;
    self->view = $View.getInstance(self->multitonKey, $View.new);
}

static void registerCommand(Facade *self, const char *notificationName, SimpleCommand *(*factory)(void)) {
    self->controller->registerCommand(self->controller, notificationName, factory);
}

static void removeCommand(Facade *self, const char *notificationName) {
    self->controller->removeCommand(self->controller, notificationName);
}

static bool hasCommand(Facade *self, const char *notificationName) {
    return self->controller->hasCommand(self->controller, notificationName);
}

static void registerProxy(Facade *self, Proxy *proxy) {
    self->model->registerProxy(self->model, proxy);
}

static Proxy *retrieveProxy(Facade *self, const char *proxyName) {
    return self->model->retrieveProxy(self->model, proxyName);
}

static Proxy *removeProxy(Facade *self, const char *proxyName) {
    return self->model->removeProxy(self->model, proxyName);
}

static bool hasProxy(Facade *self, const char *proxyName) {
    return self->model->hasProxy(self->model, proxyName);
}

static void registerMediator(Facade *self, Mediator *mediator) {
    self->view->registerMediator(self->view, mediator);
}

static Mediator *retrieveMediator(Facade *self, const char *mediatorName) {
    return self->view->retrieveMediator(self->view, mediatorName);
}

static Mediator *removeMediator(Facade *self, const char *mediatorName) {
    return self->view->removeMediator(self->view, mediatorName);
}

static bool hasMediator(Facade *self, const char *mediatorName) {
    return self->view->hasMediator(self->view, mediatorName);
}

static void sendNotification(Facade *self, const char *notificationName, void *body, char *type) {
    Notification *notification = $Notification.new(notificationName, body, type);
    self->notifyObservers(self, notification);
    $Notification.delete(notification);
}

static void notifyObservers(Facade *self, Notification *notification) {
    self->view->notifyObservers(self->view, notification);
}

static void initializeNotifier(Facade *self, const char *key) {
    self->multitonKey = key;
}

static void init(Facade *facade) {
    facade->controller = NULL;
    facade->model = NULL;
    facade->view = NULL;
    facade->initializeFacade = initializeFacade;
    facade->initializeController = initializeController;
    facade->initializeModel = initializeModel;
    facade->initializeView = initializeView;
    facade->registerCommand = registerCommand;
    facade->removeCommand = removeCommand;
    facade->hasCommand = hasCommand;
    facade->registerProxy = registerProxy;
    facade->retrieveProxy = retrieveProxy;
    facade->removeProxy = removeProxy;
    facade->hasProxy = hasProxy;
    facade->registerMediator = registerMediator;
    facade->retrieveMediator = retrieveMediator;
    facade->removeMediator = removeMediator;
    facade->hasMediator = hasMediator;
    facade->sendNotification = sendNotification;
    facade->notifyObservers = notifyObservers;
    facade->initializeNotifier = initializeNotifier;
}

static Facade *new(const char *key) {
    assert(GetFacadeMap(key) == NULL);
    Facade *facade = malloc(sizeof(Facade));
    if (facade == NULL) goto exception;
    init(facade);
    facade->multitonKey = key;
    return facade;

    exception:
        fprintf(stderr, "Facade allocation failed.\n");
        return NULL;
}

static Facade *getInstance(const char *key, Facade *(*factory)(const char *)) {
    pthread_rwlock_wrlock(&facade_mutex);
    Facade *instance = GetFacadeMap(key);
    if (instance == NULL) {
        instance = factory(key);
        AddFacadeMap(key, instance);
        instance->initializeFacade(instance);
    }
    pthread_rwlock_unlock(&facade_mutex);
    return instance;
}

static bool hasCore(const char *key) {
    pthread_rwlock_rdlock(&facade_mutex);
    bool result = GetFacadeMap(key) != NULL;
    pthread_rwlock_unlock(&facade_mutex);
    return result;
}

static void removeFacade(const char *key) {
    pthread_rwlock_wrlock(&facade_mutex);
    $Model.removeModel(key);
    $View.removeView(key);
    $Controller.removeController(key);
    RemoveFacadeMap(key);
    pthread_rwlock_unlock(&facade_mutex);
}

const struct $Facade $Facade = { .new = new, .init = init, .getInstance = getInstance, .hasCore = hasCore, .removeFacade = removeFacade };
