#pragma once

#include <_string.h>

#include "puremvc/puremvc.h"

#define ON_REGISTER_CALLED "onRegister Called"
#define ON_REMOVE_CALLED "onRemove Called"

static void onRegister(struct IProxy *self) {
    self->setData(self, strdup(ON_REGISTER_CALLED));
}

static void onRemove(struct IProxy *self) {
    self->setData(self, strdup(ON_REMOVE_CALLED));
}

struct IProxy *model_test_proxy_new(const char *name, void *data) {
    struct IProxy *proxy = puremvc_proxy_new(name, data);
    proxy->onRegister = onRegister;
    proxy->onRemove = onRemove;
    return proxy;
}