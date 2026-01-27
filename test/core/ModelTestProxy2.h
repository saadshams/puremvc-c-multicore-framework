#pragma once

#include "puremvc/puremvc.h"

#define ON_REGISTER_CALLED2 "onRegister Called2"
#define ON_REMOVE_CALLED2 "onRemove Called2"

struct ModelTestProxy2 {
    struct IProxy base;

    const char* name;
    void* data;
};

struct IProxy *model_test_proxy2_new(const char *name, void *data, const char **error);
