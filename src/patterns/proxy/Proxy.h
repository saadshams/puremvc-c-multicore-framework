#pragma once

#include "puremvc/IProxy.h"

struct Proxy {
    struct IProxy base;

    const char* name;
    void* data;
};
