#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <collection/Mutex.h>
#include <collection/IDictionary.h>

#include "Facade.h"
#include "puremvc/IController.h"
#include "puremvc/IModel.h"
#include "puremvc/IView.h"

// The Multiton Facade instanceMap.
static struct IDictionary *instanceMap;

// mutex for instanceMap
static MutexOnce token = MUTEX_ONCE_INIT;
static Mutex mutex;

static void initializeFacade(struct IFacade *self) {
    self->initializeModel(self);
    self->initializeController(self);
    self->initializeView(self);
}

static void initializeController(struct IFacade *self) {
    struct Facade *this = (struct Facade *) self;
    this->controller = puremvc_controller_getInstance(this->multitonKey, puremvc_controller_new);
}

static void initializeModel(struct IFacade *self) {
    struct Facade *this = (struct Facade *) self;
    this->model = puremvc_model_getInstance(this->multitonKey, puremvc_model_new);
}

static void initializeView(struct IFacade *self) {
    struct Facade *this = (struct Facade *) self;
    this->view = puremvc_view_getInstance(this->multitonKey, puremvc_view_new);
}

static void registerCommand(const struct IFacade *self, const char *notificationName, struct ICommand *(*factory)()) {
    const struct Facade *this = (struct Facade *) self;
    this->controller->registerCommand(this->controller, notificationName, factory);
}

static void removeCommand(const struct IFacade *self, const char *notificationName) {
    const struct Facade *this = (struct Facade *) self;
    this->controller->removeCommand(this->controller, notificationName);
}

static bool hasCommand(const struct IFacade *self, const char *notificationName) {
    const struct Facade *this = (struct Facade *) self;
    return this->controller->hasCommand(this->controller, notificationName);
}

static void registerProxy(const struct IFacade *self, struct IProxy *proxy) {
    const struct Facade *this = (struct Facade *) self;
    this->model->registerProxy(this->model, proxy);
}

static struct IProxy *retrieveProxy(const struct IFacade *self, const char *proxyName) {
    const struct Facade *this = (struct Facade *) self;
    return this->model->retrieveProxy(this->model, proxyName);
}

static struct IProxy *removeProxy(const struct IFacade *self, const char *proxyName) {
    const struct Facade *this = (struct Facade *) self;
    return this->model->removeProxy(this->model, proxyName);
}

static bool hasProxy(const struct IFacade *self, const char *proxyName) {
    const struct Facade *this = (struct Facade *) self;
    return this->model->hasProxy(this->model, proxyName);
}

static void registerMediator(const struct IFacade *self, struct IMediator *mediator) {
    const struct Facade *this = (struct Facade *) self;
    this->view->registerMediator(this->view, mediator);
}

static struct IMediator *retrieveMediator(const struct IFacade *self, const char *mediatorName) {
    const struct Facade *this = (struct Facade *) self;
    return this->view->retrieveMediator(this->view, mediatorName);
}

static struct IMediator *removeMediator(const struct IFacade *self, const char *mediatorName) {
    const struct Facade *this = (struct Facade *) self;
    return this->view->removeMediator(this->view, mediatorName);
}

static bool hasMediator(const struct IFacade *self, const char *mediatorName) {
    const struct Facade *this = (struct Facade *) self;
    return this->view->hasMediator(this->view, mediatorName);
}

static void notifyObservers(const struct IFacade *self, const struct INotification *notification) {
    const struct Facade *this = (struct Facade *) self;
    this->view->notifyObservers(this->view, notification);
}

static void sendNotification(const struct IFacade *self, const char *notificationName, void *body, const char *type) {
    struct INotification *notification = puremvc_notification_new(notificationName, body, type);
    self->notifyObservers(self, notification);
    puremvc_notification_free(&notification);
}

static struct Facade *init(struct Facade *facade) {
    if (facade == NULL) return NULL;
    facade->base.initializeFacade = initializeFacade;
    facade->base.initializeController = initializeController;
    facade->base.initializeModel = initializeModel;
    facade->base.initializeView = initializeView;
    facade->base.registerCommand = registerCommand;
    facade->base.removeCommand = removeCommand;
    facade->base.hasCommand = hasCommand;
    facade->base.registerProxy = registerProxy;
    facade->base.retrieveProxy = retrieveProxy;
    facade->base.removeProxy = removeProxy;
    facade->base.hasProxy = hasProxy;
    facade->base.registerMediator = registerMediator;
    facade->base.retrieveMediator = retrieveMediator;
    facade->base.removeMediator = removeMediator;
    facade->base.hasMediator = hasMediator;
    facade->base.notifyObservers = notifyObservers;
    facade->base.sendNotification = sendNotification;
    return facade;
}

static struct Facade *alloc(const char *key) {
    assert(instanceMap->get(instanceMap, key) == NULL);

    struct Facade *facade = malloc(sizeof(struct Facade));
    if (facade == NULL) {
        fprintf(stderr, "[PureMVC::Facade::%s] Error: Failed to allocate Facade with key '%s'.\n", __func__, key);
        return NULL;
    }
    memset(facade, 0, sizeof(*facade));

    facade->multitonKey = strdup(key);
    if (facade->multitonKey == NULL) {
        fprintf(stderr, "[PureMVC::Facade::%s] Error: strdup failed for key '%s'.\n", __func__, key);
        free(facade);
        return NULL;
    }

    return facade;
}

struct IFacade *puremvc_facade_new(const char *key) {
    assert(key != NULL);
    return (struct IFacade *) init(alloc(key));
}

void puremvc_facade_free(struct IFacade **facade) {
    if (facade == NULL || *facade == NULL) return;
    struct Facade *this = (struct Facade *) *facade;

    free((void *) this->multitonKey);

    free(this);
    *facade = NULL;
}

static void dispatchOnce() {
    mutex_init(&mutex);
}

struct IFacade *puremvc_facade_getInstance(const char *key, struct IFacade *(*factory)(const char *)) {
    assert(key != NULL);
    assert(factory != NULL);
    mutex_once(&token, dispatchOnce);
    mutex_lock(&mutex);

    if (instanceMap == NULL) instanceMap = collection_dictionary_new();

    struct IFacade *instance = (struct IFacade *) instanceMap->get(instanceMap, key);
    if (instance == NULL) {
        instance = factory(key);
        instanceMap->put(instanceMap, key, instance);
        instance->initializeFacade(instance);
    }

    mutex_unlock(&mutex);
    return instance;
}

bool puremvc_facade_hasCore(const char *key) {
    assert(key != NULL);
    mutex_lock_shared(&mutex);
    const bool result = instanceMap->containsKey(instanceMap, key);
    mutex_unlock(&mutex);
    return result;
}

void puremvc_facade_removeFacade(const char *key) {
    assert(key != NULL);
    mutex_lock(&mutex);

    puremvc_model_removeModel(key);
    puremvc_view_removeView(key);
    puremvc_controller_removeController(key);

    struct IFacade *facade = instanceMap->removeItem(instanceMap, key);
    puremvc_facade_free(&facade);

    mutex_unlock(&mutex);
}
