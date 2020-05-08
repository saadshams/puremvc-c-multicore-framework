#ifndef PUREMVC_MODEL_H
#define PUREMVC_MODEL_H

#include "Proxy.h"
#include <stdbool.h>

typedef struct ProxyMap ProxyMap;

struct ProxyMap {
    char *name;
    Proxy *proxy;
    ProxyMap *next;
};

typedef struct Model Model;

struct Model {
    char *multitonKey;
    ProxyMap *proxyMap;
    void (*initializeModel)(Model *self);
    void (*registerProxy)(Model *self, Proxy *proxy);
    Proxy *(*retrieveProxy)(const Model *self, const char *proxyName);
    bool (*hasProxy)(const Model *self, const char *proxyName);
    Proxy *(*removeProxy)(Model *self, const char *proxyName);
};

void InitModel(Model *self);

Model *NewModel(char *key);

void DeleteModel(char *key);

Model *getModelInstance(char *key, Model *(factory)(char *));

#endif //PUREMVC_MODEL_H
