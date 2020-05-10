#include "interfaces/Model.h"
#include "interfaces/Notifier.h"
#include <pthread.h>
#include <string.h>
#include <assert.h>

pthread_mutex_t model_mutex;

pthread_rwlock_t modelMap_mutex;

typedef struct ModelMap ModelMap;

struct ModelMap {
    char *name;
    Model *model;
    ModelMap *next;
};

static ModelMap *instanceMap;

static Model *GetModelMap(char *key) {
    ModelMap *cursor = instanceMap;
    while (cursor && strcmp(cursor->name, key) != 0)
        cursor = cursor->next;
    return cursor == NULL ? NULL : cursor->model;
}

static ModelMap *NewModelMap(char *key, Model *model) {
    ModelMap *self = malloc(sizeof(ModelMap));
    self->name = strdup(key);
    self->model = model;
    self->next = NULL;
    return self;
}

static void DeleteModelMap(ModelMap *self) {
    free(self->model->multitonKey);
    free(self->model);
    free(self->name);
    free(self);
}

static void AddModelMap(char *key, Model *model) {
    ModelMap **cursor = &instanceMap;
    while (*cursor)
        cursor = &(*cursor)->next;
    *cursor = NewModelMap(key, model);
}

static void RemoveModelMap(char *key) {
    ModelMap **cursor = &instanceMap;
    while (*cursor) {
        if (strcmp((*cursor)->name, key) == 0) {
            ModelMap *node = *cursor;
            *cursor = (*cursor)->next;
            DeleteModelMap(node);
            break;
        }
        cursor = &(*cursor)->next;
    }
}

static ProxyMap *NewProxyNode(Proxy *proxy) {
    ProxyMap *self = malloc(sizeof(ProxyMap));
    self->name = strdup(proxy->getProxyName(proxy));
    self->proxy = proxy;
    self->next = NULL;
    return self;
}

static void DeleteProxyNode(ProxyMap *self) {
    free(self->name);
    free(self);
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

static Proxy *retrieveProxy(const Model *self, const char *proxyName) {
    pthread_rwlock_rdlock(&modelMap_mutex);
    ProxyMap *cursor = self->proxyMap;
    while (cursor && strcmp(cursor->name, proxyName) != 0)
        cursor = cursor->next;
    pthread_rwlock_unlock(&modelMap_mutex);
    return cursor == NULL ? NULL : cursor->proxy;
}

static bool hasProxy(const Model *self, const char *proxyName) {
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
    if (self) {
        self->proxyMap = NULL;
        self->initializeModel = initializeModel;
        self->registerProxy = registerProxy;
        self->retrieveProxy = retrieveProxy;
        self->hasProxy = hasProxy;
        self->removeProxy = removeProxy;
    }
}

Model *NewModel(char *key) {
    assert(GetModelMap(key) == NULL);
    Model *self = malloc(sizeof(Model));
    InitModel(self);
    self->multitonKey = strdup(key);
    return self;
}

void DeleteModel(char *key) {
    pthread_mutex_lock(&model_mutex);
    RemoveModelMap(key);
    pthread_mutex_unlock(&model_mutex);
}

Model *getModelInstance(char *key, Model *(*factory)(char *)) {
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