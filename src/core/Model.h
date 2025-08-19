#pragma once

#include <collection/Mutex.h>

#include "puremvc/IModel.h"

struct Model {
    struct IModel base;

    const char *multitonKey;

    Mutex proxyMapMutex;
    struct IDictionary *proxyMap;
};
