#include "interfaces/Proxy.h"
#include "interfaces/Notifier.h"
#include <string.h>

static char *getProxyName(const Proxy *self) {
    return self->proxyName;
}

static void setData(Proxy *self, void *data) {
    self->data = data;
}

static void *getData(const Proxy *self) {
    return self->data;
}

static void onRegister(Proxy *self) {

}

static void onRemove(Proxy *self) {

}

void InitProxy(Proxy *self, const char *proxyName, void *data) {
    if (self) {
        self->notifier = NewNotifier();
        self->proxyName = strdup(proxyName != NULL ? proxyName : PROXY_NAME);
        self->data = data != NULL ? data : NULL;
        self->getProxyName = getProxyName;
        self->setData = setData;
        self->getData = getData;
        self->onRegister = onRegister;
        self->onRemove = onRemove;
    }
}

Proxy *NewProxy(const char *proxyName, void *data) {
    Proxy *self = malloc(sizeof(Proxy));
    InitProxy(self, proxyName, data);
    return self;
}

void DeleteProxy(Proxy *self) {
    free(self->proxyName);
    free(self);
    self = NULL;
}
