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

static void init(Proxy *proxy, const char *proxyName, void *data) {
    proxy->notifier = $Notifier.new();
    proxy->proxyName = proxyName != NULL ? proxyName : PROXY_NAME;
    proxy->data = data != NULL ? data : NULL;
    proxy->getProxyName = getProxyName;
    proxy->setData = setData;
    proxy->getData = getData;
    proxy->onRegister = onRegister;
    proxy->onRemove = onRemove;
}

static Proxy *new(const char *proxyName, void *data) {
    Proxy *proxy = malloc(sizeof(Proxy));
    if (proxy == NULL) goto exception;
    init(proxy, proxyName, data);
    return proxy;

    exception:
    fprintf(stderr, "Proxy allocation failed.\n");
    return NULL;
}

static void delete(Proxy *proxy) {
    $Notifier.delete(proxy->notifier);
    free(proxy);
    proxy = NULL;
}

const struct $Proxy $Proxy = { .new = new, .init = init, .delete = delete };
