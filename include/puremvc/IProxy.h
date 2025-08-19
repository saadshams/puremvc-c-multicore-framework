#pragma once

#include "INotifier.h"

#define PROXY_NAME "Proxy"

struct IProxy {
    struct INotifier *notifier;

    const char* (*getName)(const struct IProxy* self);
    void* (*getData)(const struct IProxy* self);
    void (*setData)(struct IProxy* self, void* data);

    void (*onRegister)(struct IProxy* self);
    void (*onRemove)(struct IProxy* self);
};

struct IProxy* puremvc_proxy_new(const char* name, void* data);
void puremvc_proxy_free(struct IProxy** proxy);
