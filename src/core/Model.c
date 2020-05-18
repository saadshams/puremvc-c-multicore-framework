#include "interfaces/Model.h"
#include "interfaces/Notifier.h"
#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>

struct ProxyMap {
    const char *name;
    Proxy *proxy;
    ProxyMap *next;
};

// ModelMap LinkedList
typedef struct ModelMap ModelMap;

struct ModelMap {
    const char *name;
    Model *model;
    ModelMap *next;
};

// The Multiton Model instanceMap.
static ModelMap *instanceMap;

// mutex for modelMap
static pthread_rwlock_t modelMap_mutex;

// Construct a new instanceMap node
static ModelMap *NewModelMap(const char *key, Model *model) {
    ModelMap *modelMap = malloc(sizeof(ModelMap));
    if (modelMap == NULL) goto exception;
    modelMap->name = key;
    modelMap->model = model;
    modelMap->next = NULL;
    return modelMap;

    exception:
        fprintf(stderr, "ModelMap allocation failed.\n");
        return NULL;
}

// Retrieve a Node from instanceMap LinkedList
static Model *GetModelMap(const char *key) {
    ModelMap *modelMap = instanceMap;
    while (modelMap && strcmp(modelMap->name, key) != 0)
        modelMap = modelMap->next;
    return modelMap == NULL ? NULL : modelMap->model;
}

// Add a Node to the instanceMap LinkedList
static void AddModelMap(const char *key, Model *model) {
    ModelMap **modelMap = &instanceMap;
    while (*modelMap)
        modelMap = &(*modelMap)->next;
    *modelMap = NewModelMap(key, model);
}

// Remove a node from instanceMap LinkedList
static void RemoveModelMap(const char *key) {
    ModelMap **modelMap = &instanceMap;
    while (*modelMap) {
        if (strcmp((*modelMap)->name, key) == 0) {
            ModelMap *node = *modelMap;
            *modelMap = (*modelMap)->next;
            free(node->model);
            free(node);
            node = NULL;
            break;
        }
        modelMap = &(*modelMap)->next;
    }
}

// Construct a new proxyMap node
static ProxyMap *NewProxyNode(Proxy *proxy) {
    ProxyMap *proxyMap = malloc(sizeof(ProxyMap));
    proxyMap->name = proxy->getProxyName(proxy);
    proxyMap->proxy = proxy;
    proxyMap->next = NULL;
    return proxyMap;
}

// Release the memory for a proxyMap node
static void DeleteProxyNode(ProxyMap *proxyMap) {
    free(proxyMap);
    proxyMap = NULL;
}

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

void InitModel(Model *model) {
    model->proxyMap = NULL;
    model->initializeModel = initializeModel;
    model->registerProxy = registerProxy;
    model->retrieveProxy = retrieveProxy;
    model->hasProxy = hasProxy;
    model->removeProxy = removeProxy;
}

Model *NewModel(const char *key) {
    assert(GetModelMap(key) == NULL);

    Model *model = malloc(sizeof(Model));
    if (model == NULL) goto exception;
    InitModel(model);
    model->multitonKey = key;
    AddModelMap(key, model);
    return model;

    exception:
        fprintf(stderr, "Model allocation failed.\n");
        return NULL;
}

// mutex for model instanceMap
static pthread_rwlock_t model_mutex;

/** Destructor */
void RemoveModel(const char *key) {
    pthread_rwlock_wrlock(&model_mutex);
    RemoveModelMap(key);
    pthread_rwlock_unlock(&model_mutex);
}

Model *getModelInstance(const char *key, Model *(*factory)(const char *)) {
    pthread_rwlock_wrlock(&model_mutex);
    Model *instance = GetModelMap(key);
    if (instance == NULL) {
        instance = factory(key);
        instance->initializeModel(instance);
    }
    pthread_rwlock_unlock(&model_mutex);
    return instance;
}
