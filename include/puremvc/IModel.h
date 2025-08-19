#pragma once

#include <stdbool.h>

#include "IProxy.h"

struct IModel {
    void (*initializeModel)(struct IModel *self);

    void (*registerProxy)(const struct IModel *self, struct IProxy *proxy);
    struct IProxy *(*retrieveProxy)(const struct IModel *self, const char *proxyName);
    bool (*hasProxy)(const struct IModel *self, const char *proxyName);
    struct IProxy *(*removeProxy)(const struct IModel *self, const char *proxyName);
};

struct IModel *puremvc_model_new(const char *key);
void puremvc_model_free(struct IModel **model);

struct IModel *puremvc_model_getInstance(const char *key, struct IModel *(*factory)(const char *));
void puremvc_model_removeModel(const char *key);
