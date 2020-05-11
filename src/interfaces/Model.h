#ifndef PUREMVC_MODEL_H
#define PUREMVC_MODEL_H

#include "Proxy.h"
#include <stdbool.h>

typedef struct ProxyMap ProxyMap;

struct ProxyMap {
    const char *name;
    Proxy *proxy;
    ProxyMap *next;
};

typedef struct Model Model;

struct Model {
    const char *multitonKey;
    ProxyMap *proxyMap;
    void (*initializeModel)(Model *self);
    void (*registerProxy)(Model *self, Proxy *proxy);
    Proxy *(*retrieveProxy)(const Model *self, const char *proxyName);
    bool (*hasProxy)(const Model *self, const char *proxyName);
    Proxy *(*removeProxy)(Model *self, const char *proxyName);
};

void InitModel(Model *self);

Model *NewModel(const char *key);

void DeleteModel(const char *key);

Model *getModelInstance(const char *key, Model *(factory)(const char *));

#endif //PUREMVC_MODEL_H
