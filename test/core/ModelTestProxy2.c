#include <string.h>
#include "ModelTestProxy2.h"

#include <stdlib.h>

static void onRegister(struct IProxy *self) {
    // self->setData(self, strdup(ON_REGISTER_CALLED2));
}

static void onRemove(struct IProxy *self) {
    // self->setData(self, strdup(ON_REMOVE_CALLED2));
}

struct ModelTestProxy2 *model_test_proxy2_new(const char *name, void *data, const char **error) {
    struct ModelTestProxy2 *proxy = malloc(sizeof(struct ModelTestProxy2));

    memset(proxy, 0, sizeof(struct ModelTestProxy2));

    struct IProxy *temp = puremvc_proxy_new(name, data, error);
    proxy->base = *temp;
    // free(temp);

    proxy->base.onRegister = onRegister;
    proxy->base.onRemove = onRemove;
    return proxy;
}
