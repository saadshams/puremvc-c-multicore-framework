#include "interfaces/Notifier.h"
#include "interfaces/Proxy.h"
#include <stdio.h>
#include <string.h>

static const char *getProxyName(const Proxy *self) {
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
    self->notifier = NewNotifier();
    self->proxyName = proxyName != NULL ? proxyName : PROXY_NAME;
    self->data = data != NULL ? data : NULL;
    self->getProxyName = getProxyName;
    self->setData = setData;
    self->getData = getData;
    self->onRegister = onRegister;
    self->onRemove = onRemove;
}

Proxy *NewProxy(const char *proxyName, void *data) {
    Proxy *self = malloc(sizeof(Proxy));
    if (self == NULL) goto exception;
    InitProxy(self, proxyName, data);
    return self;

    exception:
        fprintf(stderr, "Proxy allocation failed.\n");
        return NULL;
}

void DeleteProxy(Proxy *self) {
    DeleteNotifier(self->notifier);
    free(self);
    self = NULL;
}
