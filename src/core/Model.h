/**
* @file Model.h
* @internal
* @brief Model Header
*
* @author Saad Shams <saad.shams@puremvc.org>
* @copyright BSD 3-Clause License
*/
#pragma once

#include <collection/Mutex.h>

#include "puremvc/IModel.h"

struct Model {
    struct IModel base;

    const char *multitonKey;

    Mutex proxyMapMutex;
    struct IDictionary *proxyMap;
};
