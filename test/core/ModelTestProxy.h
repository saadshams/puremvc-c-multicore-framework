#ifndef PUREMVC_MODELTESTPROXY_H
#define PUREMVC_MODELTESTPROXY_H

#include "interfaces/Proxy.h"

#define ON_REGISTER_CALLED "onRegister Called";
#define ON_REMOVE_CALLED "onRemove Called";

typedef struct ModelTestProxy ModelTestProxy;

struct ModelTestProxy {
    Proxy proxy;
};

ModelTestProxy *NewModelTestProxy(char*, void*);

#endif //PUREMVC_MODELTESTPROXY_H
