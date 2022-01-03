#include "interfaces/Proxy.h"
#include "interfaces/Model.h"
#include "interfaces/Notifier.h"
#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>

// The Multiton Model instanceMap.
static Map *instanceMap;

// mutex for model instanceMap
static pthread_rwlock_t modelMap_mutex;

// mutex for modelMap
static pthread_rwlock_t proxyMap_mutex;

static void initializeModel(Model *self) {

}

static Proxy *registerProxy(Model *self, Proxy *proxy) {
    pthread_rwlock_wrlock(&proxyMap_mutex);
    proxy->notifier->initializeNotifier(proxy->notifier, self->multitonKey);
    if ($Map.containsKey(&self->proxyMap, proxy->proxyName)) {
        Proxy *previous = (Proxy *) $Map.get(&self->proxyMap, proxy->proxyName);
        $Map.replace(&self->proxyMap, proxy->proxyName, proxy);
        return previous;
    } else {
        $Map.put(&self->proxyMap, proxy->proxyName, proxy);
    }
    proxy->onRegister(proxy);
    pthread_rwlock_wrlock(&proxyMap_mutex);
    return NULL;
}

static Proxy *retrieveProxy(Model *self, const char *proxyName) {
    pthread_rwlock_rdlock(&proxyMap_mutex);
    Proxy *proxy = (Proxy *) $Map.get(&self->proxyMap, proxyName);
    pthread_rwlock_unlock(&proxyMap_mutex);
    return proxy;
}

static bool hasProxy(Model *self, const char *proxyName) {
    pthread_rwlock_rdlock(&proxyMap_mutex);
    bool exists = $Map.containsKey(&self->proxyMap, proxyName);
    pthread_rwlock_unlock(&proxyMap_mutex);
    return exists;
}

static Proxy *removeProxy(Model *self, const char *proxyName) {
    pthread_rwlock_wrlock(&proxyMap_mutex);
    Proxy *proxy = (Proxy *) $Map.remove(&self->proxyMap, proxyName);
    if (proxy) {
        proxy->onRemove(proxy);
    }
    pthread_rwlock_wrlock(&proxyMap_mutex);
    return proxy;
}

static void init(Model *model) {
    model->proxyMap = NULL;
    model->initializeModel = initializeModel;
    model->registerProxy = registerProxy;
    model->retrieveProxy = retrieveProxy;
    model->hasProxy = hasProxy;
    model->removeProxy = removeProxy;
}

static Model *new(const char *key) {
    assert($Map.get(&instanceMap, key) == NULL);

    Model *model = malloc(sizeof(Model));
    if (model == NULL) goto exception;
    init(model);
    model->multitonKey = key;
    return model;

    exception:
    fprintf(stderr, "Model allocation failed.\n");
    return NULL;
}

static void removeModel(const char *key) {
    pthread_rwlock_wrlock(&modelMap_mutex);
    Model *model = (Model *) $Map.remove(&instanceMap, key);
    if (model != NULL) free(model);
    pthread_rwlock_unlock(&modelMap_mutex);
}

static Model *getInstance(const char *key, Model *(*factory)(const char *)) {
    pthread_rwlock_wrlock(&modelMap_mutex);
    Model *instance = (Model *) $Map.get(&instanceMap, key);
    if (instance == NULL) {
        instance = (Model *) $Map.put(&instanceMap, key, factory(key));
        instance->initializeModel(instance);
    }
    pthread_rwlock_unlock(&modelMap_mutex);
    return instance;
}

const struct $Model $Model = {.new = new, .init = init, .getInstance = getInstance, .removeModel = removeModel};
