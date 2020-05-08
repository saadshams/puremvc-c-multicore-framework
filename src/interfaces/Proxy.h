#ifndef PUREMVC_PROXY_H
#define PUREMVC_PROXY_H

#include <stdlib.h>

struct Notifier;

typedef struct Proxy Proxy;

struct Proxy {
    struct Notifier *notifier;
    char *proxyName;
    void *data;
    char *(*getProxyName)(const Proxy *self);
    void (*setData)(Proxy *self, void *data);
    void *(*getData)(const Proxy *self);
    void (*onRegister)(Proxy *self);
    void (*onRemove)(Proxy *self);
};

static char *PROXY_NAME = "Proxy";

void InitProxy(Proxy *self, const char *proxyName, void *data);

Proxy *NewProxy(const char *proxyName, void *data);

void DeleteProxy(Proxy *self);

#endif //PUREMVC_PROXY_H
