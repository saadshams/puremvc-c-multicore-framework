/**
* @file Proxy.c
* @internal
* @brief Proxy Implementation
*
* @author Saad Shams <saad.shams@puremvc.org>
* @copyright BSD 3-Clause License
*/
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
    if (this->data != data) {
        free(this->data);
        this->data = data;
    }
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

static struct Proxy *alloc(const char *name, void *data, const char **error) {
    struct Proxy *proxy = malloc(sizeof(struct Proxy));
    if (proxy == NULL) return *error = "[PureMVC::Proxy::alloc] Error: Failed to allocate Proxy", NULL;

    memset(proxy, 0, sizeof(struct Proxy));

    proxy->base.notifier = puremvc_notifier_new(error);
    if (proxy->base.notifier == NULL) return free(proxy), NULL;

    proxy->name = strdup(name ? name : PROXY_NAME);
    if (proxy->name == NULL) return *error = "[PureMVC::Proxy::alloc] Error: Failed to allocate proxy name (strdup)",
            puremvc_notifier_free(&proxy->base.notifier), free(proxy), NULL;

    proxy->data = data;
    return proxy;
}

struct IProxy *puremvc_proxy_new(const char *name, void *data, const char **error) {
    return (struct IProxy *) init(alloc(name, data, error));
}

void puremvc_proxy_free(struct IProxy **proxy) {
    if (proxy == NULL || *proxy == NULL) return;
    struct Proxy *this = (struct Proxy *) *proxy;

    free((void *) this->name);
    free(this->data);
    puremvc_notifier_free(&this->base.notifier);

    free(this);
    *proxy = NULL;
}
