#include <string.h>
#include "ModelTestProxy.h"

static void onRegister(struct IProxy *self) {
    self->setData(self, strdup(ON_REGISTER_CALLED));
}

static void onRemove(struct IProxy *self) {
    self->setData(self, strdup(ON_REMOVE_CALLED));
}

struct IProxy *model_test_proxy_new(const char *name, void *data) {
    const char *error = NULL;
    struct IProxy *proxy = puremvc_proxy_new(name, data, &error);
    proxy->onRegister = onRegister;
    proxy->onRemove = onRemove;
    return proxy;
}
