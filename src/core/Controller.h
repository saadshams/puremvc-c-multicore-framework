#pragma once

#include <collection/Mutex.h>

#include "puremvc/IController.h"

struct Controller {
    struct IController base;

    const char *multitonKey;
    struct IView *view;

    Mutex commandMapMutex;
    struct IDictionary *commandMap;
};
