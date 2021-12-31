#include "interfaces/Model.h"
#include "interfaces/Notifier.h"
#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>

struct ProxyNode {
    const char *name;
    Proxy *proxy;
    ProxyNode *next;
};

// ModelNode LinkedList
typedef struct ModelNode ModelNode;

struct ModelNode {
    const char *name;
    Model *model;
    ModelNode *next;
};

// The Multiton Model instanceMap.
static ModelNode *instanceMap;

// mutex for modelMap
static pthread_rwlock_t modelMap_mutex;

// Construct a new instanceMap node
static ModelNode *NewModelNode(const char *key, Model *model) {
    ModelNode *node = malloc(sizeof(ModelNode));
    if (node == NULL) goto exception;
    node->name = key;
    node->model = model;
    node->next = NULL;
    return node;

    exception:
        fprintf(stderr, "ModelNode allocation failed.\n");
        return NULL;
}

// Retrieve a Node from instanceMap LinkedList
static Model *GetModelMap(const char *key) {
    ModelNode *cursor = instanceMap;
    while (cursor && strcmp(cursor->name, key) != 0)
        cursor = cursor->next;
    return cursor != NULL ? cursor->model : NULL;
}

// Add a Node to the instanceMap LinkedList
static void AddModelNode(const char *key, Model *model) {
    ModelNode **cursor = &instanceMap;
    while (*cursor)
        cursor = &(*cursor)->next;
    *cursor = NewModelNode(key, model);
}

// Remove a node from instanceMap LinkedList
static void RemoveModelNode(const char *key) {
    ModelNode **cursor = &instanceMap;
    while (*cursor) {
        if (strcmp((*cursor)->name, key) == 0) {
            ModelNode *node = *cursor;
            *cursor = (*cursor)->next;
            free(node->model);
            free(node);
            node = NULL;
            break;
        }
        cursor = &(*cursor)->next;
    }
}

// Construct a new proxy node
static ProxyNode *NewProxyNode(Proxy *proxy) {
    ProxyNode *node = malloc(sizeof(ProxyNode));
    node->name = proxy->getProxyName(proxy);
    node->proxy = proxy;
    node->next = NULL;
    return node;
}

// Release the memory for a node node
static void DeleteProxyNode(ProxyNode *node) {
    free(node);
    node = NULL;
}

static void initializeModel(Model *self) {

}

static void registerProxy(Model *self, Proxy *proxy) {
    pthread_rwlock_wrlock(&modelMap_mutex);
    ProxyNode **cursor = &self->proxyMap;

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
    ProxyNode *cursor = self->proxyMap;
    while (cursor && strcmp(cursor->name, proxyName) != 0)
        cursor = cursor->next;
    pthread_rwlock_unlock(&modelMap_mutex);
    return cursor != NULL ? cursor->proxy : NULL;
}

static bool hasProxy(Model *self, const char *proxyName) {
    pthread_rwlock_rdlock(&modelMap_mutex);
    ProxyNode *cursor = self->proxyMap;
    while (cursor && strcmp(cursor->name, proxyName) != 0)
        cursor = cursor->next;
    pthread_rwlock_unlock(&modelMap_mutex);
    return cursor != NULL;
}

static Proxy *removeProxy(Model *self, const char *proxyName) {
    pthread_rwlock_wrlock(&modelMap_mutex);
    ProxyNode **cursor = &self->proxyMap;
    while (*cursor) {
        if (strcmp((*cursor)->name, proxyName) == 0) {
            ProxyNode *node = *cursor;
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
    AddModelNode(key, model);
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
    RemoveModelNode(key);
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
