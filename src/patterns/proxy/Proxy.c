#include "interfaces/Notifier.h"
#include "interfaces/Proxy.h"
#include <stdio.h>
#include <string.h>

/**
 * Get the proxy name
 *
 * @param self
 */
static const char *getProxyName(Proxy *self) {
    return self->proxyName;
}

/**
 * Set the data object
 *
 * @param self
 * @param data
 */
static void setData(Proxy *self, void *data) {
    self->data = data;
}

/**
 * Get the data object
 *
 * @param self
 * @return data
 */
static void *getData(Proxy *self) {
    return self->data;
}

/**
 * Called by the Model when the Proxy is registered
 *
 * @param self
 */
static void onRegister(Proxy *self) {

}

/**
 * Called by the Model when the Proxy is removed
 *
 * @param self
 */
static void onRemove(Proxy *self) {

}

/**
 * Initializer
 *
 * @param proxy
 * @param proxyName
 * @param data
 */
void InitProxy(Proxy *proxy, const char *proxyName, void *data) {
    proxy->notifier = NewNotifier();
    proxy->proxyName = proxyName != NULL ? proxyName : PROXY_NAME;
    proxy->data = data != NULL ? data : NULL;
    proxy->getProxyName = getProxyName;
    proxy->setData = setData;
    proxy->getData = getData;
    proxy->onRegister = onRegister;
    proxy->onRemove = onRemove;
}

/**
 * Constructor
 *
 * @param proxyName
 * @param data
 */
Proxy *NewProxy(const char *proxyName, void *data) {
    Proxy *proxy = malloc(sizeof(Proxy));
    if (proxy == NULL) goto exception;
    InitProxy(proxy, proxyName, data);
    return proxy;

    exception:
        fprintf(stderr, "Proxy allocation failed.\n");
        return NULL;
}

/**
 * Destructor
 *
 * @param proxy
 */
void DeleteProxy(Proxy *proxy) {
    DeleteNotifier(proxy->notifier);
    free(proxy);
    proxy = NULL;
}
