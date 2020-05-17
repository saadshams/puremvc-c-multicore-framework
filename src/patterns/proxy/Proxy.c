#include "interfaces/Notifier.h"
#include "interfaces/Proxy.h"
#include <stdio.h>
#include <string.h>

static const char *getProxyName(Proxy *self) {
    return self->proxyName;
}

static void setData(Proxy *self, void *data) {
    self->data = data;
}

/**
 * Get the data object
 */
static void *getData(Proxy *self) {
    return self->data;
}

/**
 * Called by the Model when the Proxy is registered
 */
static void onRegister(Proxy *self) {

}

/**
 * Called by the Model when the Proxy is removed
 */
static void onRemove(Proxy *self) {

}

/** Initializer */
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

/** Constructor */
Proxy *NewProxy(const char *proxyName, void *data) {
    Proxy *proxy = malloc(sizeof(Proxy));
    if (proxy == NULL) goto exception;
    InitProxy(proxy, proxyName, data);
    return proxy;

    exception:
        fprintf(stderr, "Proxy allocation failed.\n");
        return NULL;
}

/** Destructor */
void DeleteProxy(Proxy *proxy) {
    DeleteNotifier(proxy->notifier);
    free(proxy);
    proxy = NULL;
}
