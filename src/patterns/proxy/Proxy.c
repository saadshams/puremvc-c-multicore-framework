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

static void *getData(Proxy *self) {
    return self->data;
}

static void onRegister(Proxy *self) {

}

static void onRemove(Proxy *self) {

}

Proxy *NewProxy(const char *proxyName, void *data) {
    Proxy *proxy = malloc(sizeof(Proxy));
    if (proxy == NULL) goto exception;
    InitProxy(proxy, proxyName, data);
    return proxy;

    exception:
        fprintf(stderr, "Proxy allocation failed.\n");
        return NULL;
}

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

void DeleteProxy(Proxy *proxy) {
    DeleteNotifier(proxy->notifier);
    free(proxy);
    proxy = NULL;
}
