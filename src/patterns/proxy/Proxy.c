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
 * @param self
 * @param proxyName
 * @param data
 */
void InitProxy(Proxy *self, const char *proxyName, void *data) {
    self->notifier = NewNotifier();
    self->proxyName = proxyName != NULL ? proxyName : PROXY_NAME;
    self->data = data != NULL ? data : NULL;
    self->getProxyName = getProxyName;
    self->setData = setData;
    self->getData = getData;
    self->onRegister = onRegister;
    self->onRemove = onRemove;
}

/**
 * Constructor
 *
 * @param proxyName
 * @param data
 */
Proxy *NewProxy(const char *proxyName, void *data) {
    Proxy *self = malloc(sizeof(Proxy));
    if (self == NULL) goto exception;
    InitProxy(self, proxyName, data);
    return self;

    exception:
        fprintf(stderr, "Proxy allocation failed.\n");
        return NULL;
}

/**
 * Destructor
 *
 * @param self
 */
void DeleteProxy(Proxy *self) {
    DeleteNotifier(self->notifier);
    free(self);
    self = NULL;
}
