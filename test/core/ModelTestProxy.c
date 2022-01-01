#include "ModelTestProxy.h"
#include <stdlib.h>

static void onRegister(Proxy *self) {
    self->data = (void *)ON_REGISTER_CALLED;
}

static void onRemove(Proxy *self) {
    self->data = (void *)ON_REMOVE_CALLED;
}

ModelTestProxy *NewModelTestProxy(char *proxyName, void *data) {
    ModelTestProxy *self = malloc(sizeof(ModelTestProxy));
    $Proxy.init(&self->proxy, proxyName, data);
    self->proxy.onRegister = onRegister;
    self->proxy.onRemove = onRemove;
    return self;
}
