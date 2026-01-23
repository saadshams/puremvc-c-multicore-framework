/**
* @file Model.c
* @internal
* @brief Model Implementation
*
* @author Saad Shams <saad.shams@puremvc.org>
* @copyright BSD 3-Clause License
*/
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

static void initializeModel(struct IModel *self, const char **error) {

}

static void registerProxy(const struct IModel *self, struct IProxy *proxy, const char **error) {
    struct Model *this = (struct Model *) self;

    mutex_lock(&this->proxyMapMutex);
    proxy->notifier->initializeNotifier(proxy->notifier, this->multitonKey, error);
    if (*error != NULL) return mutex_unlock(&this->proxyMapMutex), (void)0;

    if (this->proxyMap->containsKey(this->proxyMap, proxy->getName(proxy))) {
        struct IProxy *previous = this->proxyMap->replace(this->proxyMap, proxy->getName(proxy), proxy);
        puremvc_proxy_free(&previous);
    } else {
        this->proxyMap->put(this->proxyMap, proxy->getName(proxy), proxy, error);
        if (*error != NULL) return mutex_unlock(&this->proxyMapMutex), (void)0;
    }
    mutex_unlock(&this->proxyMapMutex);

    proxy->onRegister(proxy);
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
    mutex_unlock(&this->proxyMapMutex);

    if (proxy != NULL)
        proxy->onRemove(proxy);

    return proxy;
}

static struct Model *init(struct Model *model) {
    if (model == NULL) return NULL;
    model->base.initializeModel = initializeModel;
    model->base.registerProxy = registerProxy;
    model->base.retrieveProxy = retrieveProxy;
    model->base.hasProxy = hasProxy;
    model->base.removeProxy = removeProxy;
    return model;
}

static struct Model *alloc(const char *key, const char **error) {
    struct Model *model = malloc(sizeof(struct Model));
    if (model == NULL) return *error = "[PureMVC::Model::alloc] Error: Failed to allocate Model", NULL;

    memset(model, 0, sizeof(struct Model));

    model->multitonKey = strdup(key);
    if (model->multitonKey == NULL) return *error = "[PureMVC::Model::alloc] Error: Failed to allocate Model key (strdup)", free(model), NULL;

    mutex_init(&model->proxyMapMutex);

    model->proxyMap = collection_dictionary_new(error);
    if (*error != NULL) return mutex_destroy(&model->proxyMapMutex), free((void *)model->multitonKey), free(model), NULL;

    return model;
}

struct IModel *puremvc_model_new(const char *key, const char **error) {
    if (key == NULL) return *error = "[PureMVC::Model::new] Error: key must not be NULL.", NULL;
    return (struct IModel *) init(alloc(key, error));
}

void puremvc_model_free(struct IModel **model) {
    if (model == NULL || *model == NULL) return;
    struct Model *this = (struct Model *) *model;

    free((void *) this->multitonKey);
    this->proxyMap->clear(this->proxyMap, free);
    collection_dictionary_free(&this->proxyMap);

    mutex_destroy(&this->proxyMapMutex);

    free(this);
    *model = NULL;
}

static void dispatchOnce() {
    mutex_init(&mutex);
}

struct IModel *puremvc_model_getInstance(const char *key, struct IModel *(*factory)(const char *key, const char **error), const char **error) {
    if (key == NULL || factory == NULL) return *error = "[PureMVC::Model::getInstance] Error: key or factory must not be NULL.", NULL;
    mutex_once(&token, dispatchOnce);
    mutex_lock(&mutex);

    if (instanceMap == NULL) {
        instanceMap = collection_dictionary_new(error);
        if (*error != NULL) return NULL;
    }

    struct IModel *instance = (struct IModel *) instanceMap->get(instanceMap, key);
    if (instance == NULL) {
        instance = factory(key, error);
        if (*error != NULL) return mutex_unlock(&mutex), NULL;

        instance->initializeModel(instance, error);
        if (*error != NULL) return puremvc_model_free(&instance), mutex_unlock(&mutex), NULL;

        instanceMap->put(instanceMap, key, instance, error);
        if (*error != NULL) return puremvc_model_free(&instance), mutex_unlock(&mutex), NULL;
    }

    mutex_unlock(&mutex);
    return instance;
}

void puremvc_model_removeModel(const char *key) {
    if (key == NULL) return;
    mutex_lock(&mutex);

    struct IModel *model = instanceMap->removeItem(instanceMap, key);
    if (model != NULL) puremvc_model_free(&model);

    mutex_unlock(&mutex);
}
