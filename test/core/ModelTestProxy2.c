#include <string.h>
#include "ModelTestProxy2.h"

#include <stdlib.h>

static void onRegister(struct IProxy *self) {
    self->setData(self, strdup(ON_REGISTER_CALLED2));
}

static void onRemove(struct IProxy *self) {
    self->setData(self, strdup(ON_REMOVE_CALLED2));
}

struct ModelTestProxy2 *model_test_proxy2_new(const char *name, void *data, const char **error) {
    struct ModelTestProxy2 *proxy = malloc(sizeof(struct ModelTestProxy2));

    memset(proxy, 0, sizeof(struct ModelTestProxy2));

    proxy->base.notifier = puremvc_notifier_new(error);

    proxy->name = strdup(name);
    proxy->data = NULL;

    puremvc_proxy_init(&proxy->base);
    proxy->base.onRegister = onRegister;
    proxy->base.onRemove = onRemove;
    return proxy;
}
