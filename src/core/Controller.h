/**
* @file Controller.h
* @internal
* @brief Controller Header
*
* @author Saad Shams <saad.shams@puremvc.org>
* @copyright BSD 3-Clause License
*/
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
