#pragma once

#include <collection/Mutex.h>

#include "puremvc/IView.h"

struct View {
    struct IView base;

    const char *multitonKey;

    // mutex for mediatorMap
    Mutex mediatorMapMutex;
    struct IDictionary *mediatorMap;

    // mutex for observerMap
    Mutex observerMapMutex;
    struct IDictionary *observerMap;
};
