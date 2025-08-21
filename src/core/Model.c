#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <collection/IDictionary.h>

#include "Model.h"

// The Multiton Model instanceMap.
static struct IDictionary *instanceMap;

// mutex for instanceMap
static MutexOnce token = MUTEX_ONCE_INIT;
static Mutex mutex;

static void initializeModel(struct IModel *self) {

}

static void registerProxy(const struct IModel *self, struct IProxy *proxy) {
    struct Model *this = (struct Model *) self;
    mutex_lock(&this->proxyMapMutex);
    proxy->notifier->initializeNotifier(proxy->notifier, this->multitonKey);
    if (this->proxyMap->containsKey(this->proxyMap, proxy->getName(proxy))) {
        struct IProxy *previous = (struct IProxy *)this->proxyMap->get(this->proxyMap, proxy->getName(proxy));
        this->proxyMap->replace(this->proxyMap, proxy->getName(proxy), proxy);
        puremvc_proxy_free(&previous);
    } else {
        this->proxyMap->put(this->proxyMap, proxy->getName(proxy), proxy);
    }
    proxy->onRegister(proxy);
    mutex_unlock(&this->proxyMapMutex);
}

static struct IProxy *retrieveProxy(const struct IModel *self, const char *proxyName) {
    struct Model *this = (struct Model *) self;
    mutex_lock_shared(&this->proxyMapMutex);
    struct IProxy *proxy = (struct IProxy *) this->proxyMap->get(this->proxyMap, proxyName);
    mutex_unlock(&this->proxyMapMutex);
    return proxy;
}

static bool hasProxy(const struct IModel *self, const char *proxyName) {
    struct Model *this = (struct Model *) self;
    mutex_lock_shared(&this->proxyMapMutex);
    const bool exists = this->proxyMap->containsKey(this->proxyMap, proxyName);
    mutex_unlock(&this->proxyMapMutex);
    return exists;
}

static struct IProxy *removeProxy(const struct IModel *self, const char *proxyName) {
    struct Model *this = (struct Model *) self;
    mutex_lock(&this->proxyMapMutex);
    struct IProxy *proxy = this->proxyMap->removeItem(this->proxyMap, proxyName);
    if (proxy != NULL)
        proxy->onRemove(proxy);
    mutex_unlock(&this->proxyMapMutex);
    return proxy;
}

static struct Model *init(struct Model *model) {
    if (model == NULL) return NULL;
    mutex_init(&model->proxyMapMutex);
    model->base.initializeModel = initializeModel;
    model->base.registerProxy = registerProxy;
    model->base.retrieveProxy = retrieveProxy;
    model->base.hasProxy = hasProxy;
    model->base.removeProxy = removeProxy;
    return model;
}

static struct Model *alloc(const char *key) {
    assert(instanceMap->get(instanceMap, key) == NULL);

    struct Model *model = malloc(sizeof(struct Model));
    if (model == NULL) {
        fprintf(stderr, "Model allocation failed.\n");
        return NULL;
    }

    memset(model, 0, sizeof(struct Model));

    model->multitonKey = strdup(key);
    model->proxyMap = collection_dictionary_new();
    return model;
}

struct IModel *puremvc_model_new(const char *key) {
    return (struct IModel *) init(alloc(key));
}

void puremvc_model_free(struct IModel **model) {
    if (model == NULL || *model == NULL) return;

    struct Model *this = (struct Model *) *model;
    free((void *)this->multitonKey);
    this->proxyMap->clear(this->proxyMap, free);
    collection_dictionary_free(&this->proxyMap);
    mutex_destroy(&this->proxyMapMutex);
    free(this);

    *model = NULL;
}

static void dispatchOnce() {
    mutex_init(&mutex);
}

struct IModel *puremvc_model_getInstance(const char *key, struct IModel *(*factory)(const char *)) {
    mutex_once(&token, dispatchOnce);
    mutex_lock(&mutex);

    if (instanceMap == NULL) instanceMap = collection_dictionary_new();

    struct IModel *instance = (struct IModel *) instanceMap->get(instanceMap, key);
    if (instance == NULL) {
        instance = factory(key);
        instanceMap->put(instanceMap, key, instance);
        instance->initializeModel(instance);
    }
    mutex_unlock(&mutex);
    return instance;
}

void puremvc_model_removeModel(const char *key) {
    mutex_lock(&mutex);
    struct IModel *model = instanceMap->removeItem(instanceMap, key);
    if (model != NULL) puremvc_model_free(&model);
    mutex_unlock(&mutex);
}
