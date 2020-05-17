#include "interfaces/Model.h"
#include "interfaces/Notifier.h"
#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>

// mutex for modelMap
static pthread_rwlock_t modelMap_mutex;

// ModelMap LinkedList
typedef struct ModelMap ModelMap;

struct ModelMap {
    const char *name;
    Model *model;
    ModelMap *next;
};

// The Multiton Model instanceMap.
static ModelMap *instanceMap;

/**
 * Construct a new instanceMap node
 *
 * @param key
 * @param model
 * @return
 */
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

/**
 * Retrieve a Node from instanceMap LinkedList
 *
 * @param key
 * @return
 */
static Model *GetModelMap(const char *key) {
    ModelMap *modelMap = instanceMap;
    while (modelMap && strcmp(modelMap->name, key) != 0)
        modelMap = modelMap->next;
    return modelMap == NULL ? NULL : modelMap->model;
}

/**
 * Add a Node to the instanceMap LinkedList
 *
 * @param key
 * @param model
 */
static void AddModelMap(const char *key, Model *model) {
    ModelMap **modelMap = &instanceMap;
    while (*modelMap)
        modelMap = &(*modelMap)->next;
    *modelMap = NewModelMap(key, model);
}

/**
 * Remove a node from instanceMap LinkedList
 *
 * @param key
 */
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

/**
 * Construct a new proxyMap node
 *
 * @param proxy
 * @return
 */
static ProxyMap *NewProxyNode(Proxy *proxy) {
    ProxyMap *proxyMap = malloc(sizeof(ProxyMap));
    proxyMap->name = proxy->getProxyName(proxy);
    proxyMap->proxy = proxy;
    proxyMap->next = NULL;
    return proxyMap;
}

/**
 * Release the memory for a proxyMap node
 *
 * @param proxyMap
 */
static void DeleteProxyNode(ProxyMap *proxyMap) {
    free(proxyMap);
    proxyMap = NULL;
}

/**
 * <P>Initialize the <code>Model</code> instance.</P>
 *
 * <P>Called automatically by the constructor, this
 * is your opportunity to initialize the Singleton
 * instance in your subclass without overriding the
 * constructor.</P>
 */
static void initializeModel(Model *self) {

}

/**
 * <P>Register an <code>IProxy</code> with the <code>Model</code>.</P>
 *
 * @param proxy an <code>IProxy</code> to be held by the <code>Model</code>.
 */
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

/**
 * <P>Retrieve an <code>IProxy</code> from the <code>Model</code>.</P>
 *
 * @param proxyName proxy name
 * @return the <code>IProxy</code> instance previously registered with the given <code>proxyName</code>.
 */
static Proxy *retrieveProxy(Model *self, const char *proxyName) {
    pthread_rwlock_rdlock(&modelMap_mutex);
    ProxyMap *cursor = self->proxyMap;
    while (cursor && strcmp(cursor->name, proxyName) != 0)
        cursor = cursor->next;
    pthread_rwlock_unlock(&modelMap_mutex);
    return cursor == NULL ? NULL : cursor->proxy;
}

/**
 * <P>Check if a Proxy is registered</P>
 *
 * @param proxyName proxy name
 * @return whether a Proxy is currently registered with the given <code>proxyName</code>.
 */
static bool hasProxy(Model *self, const char *proxyName) {
    pthread_rwlock_rdlock(&modelMap_mutex);
    ProxyMap *cursor = self->proxyMap;
    while (cursor && strcmp(cursor->name, proxyName) != 0)
        cursor = cursor->next;
    pthread_rwlock_unlock(&modelMap_mutex);
    return cursor != NULL;
}

/**
 * <P>Remove an <code>IProxy</code> from the <code>Model</code>.</P>
 *
 * @param proxyName name of the <code>IProxy</code> instance to be removed.
 * @return the <code>IProxy</code> that was removed from the <code>Model</code>
 */
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

/**
 * Initializer
 *
 * @param model
 */
void InitModel(Model *model) {
    model->proxyMap = NULL;
    model->initializeModel = initializeModel;
    model->registerProxy = registerProxy;
    model->retrieveProxy = retrieveProxy;
    model->hasProxy = hasProxy;
    model->removeProxy = removeProxy;
}

/**
 * Constructor
 *
 * @param key
 * @return Model*
 */
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

/**
 * Destructor
 *
 * @param key
 */
void RemoveModel(const char *key) {
    pthread_rwlock_wrlock(&model_mutex);
    RemoveModelMap(key);
    pthread_rwlock_unlock(&model_mutex);
}

/**
 * <P><code>Model</code> Multiton Factory method.</P>
 *
 * @param key multitonKey
 * @param factory factory that returns <code>IModel</code>
 * @return the Multiton instance of <code>Model</code>
 */
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