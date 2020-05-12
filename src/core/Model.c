#include "interfaces/Model.h"
#include "interfaces/Notifier.h"
#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>

static pthread_mutex_t model_mutex;

static pthread_rwlock_t modelMap_mutex;

// ModelMap

typedef struct ModelMap ModelMap;

struct ModelMap {
    const char *name;
    Model *model;
    ModelMap *next;
};

static ModelMap *instanceMap;

static ModelMap *NewModelMap(const char *key, Model *model) {
    ModelMap *self = malloc(sizeof(ModelMap));
    if (self == NULL) goto exception;
    self->name = key;
    self->model = model;
    self->next = NULL;
    return self;

    exception:
        fprintf(stderr, "ModelMap allocation failed.\n");
        return NULL;
}

static void AddModelMap(const char *key, Model *model) {
    ModelMap **modelMap = &instanceMap;
    while (*modelMap)
        modelMap = &(*modelMap)->next;
    *modelMap = NewModelMap(key, model);
}

static Model *GetModelMap(const char *key) {
    ModelMap *modelMap = instanceMap;
    while (modelMap && strcmp(modelMap->name, key) != 0)
        modelMap = modelMap->next;
    return modelMap == NULL ? NULL : modelMap->model;
}

static void DeleteModelMap(ModelMap *self) {
    free(self->model);
    free(self);
    self = NULL;
}

static void RemoveModelMap(const char *key) {
    ModelMap **modelMap = &instanceMap;
    while (*modelMap) {
        if (strcmp((*modelMap)->name, key) == 0) {
            ModelMap *node = *modelMap;
            *modelMap = (*modelMap)->next;
            DeleteModelMap(node);
            break;
        }
        modelMap = &(*modelMap)->next;
    }
}

// ProxyMap

static ProxyMap *NewProxyNode(Proxy *proxy) {
    ProxyMap *self = malloc(sizeof(ProxyMap));
    self->name = proxy->getProxyName(proxy);
    self->proxy = proxy;
    self->next = NULL;
    return self;
}

static void DeleteProxyNode(ProxyMap *self) {
    free(self);
    self = NULL;
}

// Model

static void initializeModel(Model *self) {

}

static void registerProxy(Model *self, Proxy *proxy) {
    pthread_rwlock_wrlock(&modelMap_mutex);
    ProxyMap **cursor = &self->proxyMap;

    while (*cursor) {
        if (strcmp((*cursor)->name, proxy->getProxyName(proxy)) == 0) {
            DeleteProxy((*cursor)->proxy);
            (*cursor)->proxy = proxy;
            return;
        }
        cursor = &(*cursor)->next;
    }
    proxy->notifier->initializeNotifier(proxy->notifier, self->multitonKey);
    *cursor = NewProxyNode(proxy);
    proxy->onRegister(proxy);
    pthread_rwlock_unlock(&modelMap_mutex);
}

static Proxy *retrieveProxy(Model *self, const char *proxyName) {
    pthread_rwlock_rdlock(&modelMap_mutex);
    ProxyMap *cursor = self->proxyMap;
    while (cursor && strcmp(cursor->name, proxyName) != 0)
        cursor = cursor->next;
    pthread_rwlock_unlock(&modelMap_mutex);
    return cursor == NULL ? NULL : cursor->proxy;
}

static bool hasProxy(Model *self, const char *proxyName) {
    pthread_rwlock_rdlock(&modelMap_mutex);
    ProxyMap *cursor = self->proxyMap;
    while (cursor && strcmp(cursor->name, proxyName) != 0)
        cursor = cursor->next;
    pthread_rwlock_unlock(&modelMap_mutex);
    return cursor != NULL;
}

static Proxy *removeProxy(Model *self, const char *proxyName) {
    pthread_rwlock_wrlock(&modelMap_mutex);
    ProxyMap **cursor = &self->proxyMap;
    while (*cursor) {
        if (strcmp((*cursor)->name, proxyName) == 0) {
            ProxyMap *node = *cursor;
            Proxy *proxy = node->proxy;
            *cursor = (*cursor)->next;
            DeleteProxyNode(node);
            proxy->onRemove(proxy);
            return proxy;
        }
        cursor = &(*cursor)->next;
    }
    pthread_rwlock_unlock(&modelMap_mutex);
    return NULL;
}

void InitModel(Model *self) {
    self->proxyMap = NULL;
    self->initializeModel = initializeModel;
    self->registerProxy = registerProxy;
    self->retrieveProxy = retrieveProxy;
    self->hasProxy = hasProxy;
    self->removeProxy = removeProxy;
}

Model *NewModel(const char *key) {
    assert(GetModelMap(key) == NULL);
    Model *self = malloc(sizeof(Model));
    if (self == NULL) goto exception;
    InitModel(self);
    self->multitonKey = key;
    return self;

    exception:
        fprintf(stderr, "Model allocation failed.\n");
        return NULL;
}

void DeleteModel(const char *key) {
    pthread_mutex_lock(&model_mutex);
    RemoveModelMap(key);
    pthread_mutex_unlock(&model_mutex);
}

Model *getModelInstance(const char *key, Model *(*factory)(const char *)) {
    pthread_mutex_lock(&model_mutex);
    Model *instance = GetModelMap(key);
    if (instance == NULL) {
        instance = factory(key);
        AddModelMap(key, instance);
        instance->initializeModel(instance);
    }
    pthread_mutex_unlock(&model_mutex);
    return instance;
}