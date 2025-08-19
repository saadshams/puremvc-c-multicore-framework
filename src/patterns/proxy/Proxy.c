#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Proxy.h"

static const char *getName(const struct IProxy *self) {
    const struct Proxy *this = (struct Proxy *) self;
    return this->name;
}

static void *getData(const struct IProxy *self) {
    const struct Proxy *this = (struct Proxy *) self;
    return this->data;
}

static void setData(struct IProxy *self, void *data) {
    struct Proxy *this = (struct Proxy *) self;
    free(this->data);
    this->data = data;
}

static void onRegister(struct IProxy *self) {

}

static void onRemove(struct IProxy *self) {

}

static struct Proxy *init(struct Proxy *proxy) {
    if (proxy == NULL) return NULL;
    proxy->base.getName = getName;
    proxy->base.getData = getData;
    proxy->base.setData = setData;
    proxy->base.onRegister = onRegister;
    proxy->base.onRemove = onRemove;
    return proxy;
}

static struct Proxy *alloc(const char *name, void *data) {
    struct Proxy *proxy = malloc(sizeof(struct Proxy));
    if (proxy == NULL) {
        fprintf(stderr, "Proxy allocation failed.\n");
        return NULL;
    }

    memset(proxy, 0, sizeof(struct Proxy));
    proxy->base.notifier = puremvc_notifier_new();
    proxy->name = strdup(name ? name : PROXY_NAME);
    if (proxy->name == NULL) {
        fprintf(stderr, "Proxy allocation failed: strdup failed\n");
        free(proxy);
        return NULL;
    }
    proxy->data = data;
    return proxy;
}

struct IProxy *puremvc_proxy_new(const char *name, void *data) {
    return (struct IProxy *)init(alloc(name, data));
}

void puremvc_proxy_free(struct IProxy **proxy) {
    if (proxy == NULL || *proxy == NULL) return;

    struct Proxy *this = (struct Proxy *)*proxy;
    free((void *)this->name);
    free(this->data);
    puremvc_notifier_free(&this->base.notifier);
    free(this);

    *proxy = NULL;
}
