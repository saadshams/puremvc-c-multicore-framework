#include "interfaces/Facade.h"
#include <assert.h>
#include <pthread.h>
#include <string.h>

static pthread_mutex_t facade_mutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct FacadeMap FacadeMap;

struct FacadeMap {
    char *name;
    Facade *facade;
    FacadeMap *next;
};

static FacadeMap *instanceMap;

static FacadeMap *NewFacadeMap(char *key, Facade *facade) {
    FacadeMap *self = malloc(sizeof(FacadeMap));
    self->name = strdup(key);
    self->facade = facade;
    self->next = NULL;
    return self;
}

static void AddFacadeMap(char *key, Facade *facade) {
    FacadeMap **cursor = &instanceMap;
    while (*cursor)
        cursor = &(*cursor)->next;
    *cursor = NewFacadeMap(key, facade);
}

static Facade *GetFacadeMap(char *key) {
    FacadeMap *cursor = instanceMap;
    while (cursor && strcmp(cursor->name, key) != 0)
        cursor = cursor->next;
    return cursor == NULL ? NULL : cursor->facade;
}

static void RemoveFacadeMap(char *key) {
    FacadeMap **cursor = &instanceMap;
    while (*cursor) {
        if (strcmp((*cursor)->name, key) == 0) {
            *cursor = (*cursor)->next;
            free((*cursor)->facade);
            free(*cursor);
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
    self->controller = getControllerInstance(self->multitonKey, NewController);
}

static void initializeModel(Facade *self) {
    if (self->model != NULL) return;
    self->model = getModelInstance(self->multitonKey, NewModel);
}

static void initializeView(Facade *self) {
    if (self->view != NULL) return;
    self->view = getViewInstance(self->multitonKey, NewView);
}

static void registerCommand(Facade *self, const char *notificationName, SimpleCommand *(*factory)()) {
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
    Notification *notification = NewNotification(notificationName, body, type);
    self->notifyObservers(self, notification);
    DeleteNotification(notification);
}

static void notifyObservers(Facade *self, Notification *notification) {
    self->view->notifyObservers(self->view, notification);
}

static void initializeNotifier(Facade *self, char *key) {
    self->multitonKey = key;
}

void InitFacade(Facade *self) {
    if (self) {
        self->controller = NULL;
        self->model = NULL;
        self->view = NULL;
        self->initializeFacade = initializeFacade;
        self->initializeController = initializeController;
        self->initializeModel = initializeModel;
        self->initializeView = initializeView;
        self->registerCommand = registerCommand;
        self->removeCommand = removeCommand;
        self->hasCommand = hasCommand;
        self->registerProxy = registerProxy;
        self->retrieveProxy = retrieveProxy;
        self->removeProxy = removeProxy;
        self->hasProxy = hasProxy;
        self->registerMediator = registerMediator;
        self->retrieveMediator = retrieveMediator;
        self->removeMediator = removeMediator;
        self->hasMediator = hasMediator;
        self->sendNotification = sendNotification;
        self->notifyObservers = notifyObservers;
        self->initializeNotifier = initializeNotifier;
    }
}

Facade *NewFacade(char *key) {
    assert(GetFacadeMap(key) == NULL);
    Facade *self = malloc(sizeof(Facade));
    InitFacade(self);
    self->multitonKey = strdup(key);
    return self;
}

bool HasCore(char *key) {
    return GetFacadeMap(key) != NULL;
}

void RemoveCore(char *key) {
    RemoveFacadeMap(key);
}

void DeleteFacade(char *key) {
    pthread_mutex_lock(&facade_mutex);
    RemoveFacadeMap(key);
    pthread_mutex_unlock(&facade_mutex);
}

Facade *getFacadeInstance(char *key, Facade *(*factory)(char *)) {
    pthread_mutex_lock(&facade_mutex);
    Facade *instance = GetFacadeMap(key);
    if (instance == NULL) {
        instance = factory(key);
        AddFacadeMap(key, instance);
        instance->initializeFacade(instance);
    }
    pthread_mutex_unlock(&facade_mutex);
    return instance;
}
