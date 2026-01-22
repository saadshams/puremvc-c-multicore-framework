/**
* @file View.h
* @internal
* @brief View Header
*
* @author Saad Shams <saad.shams@puremvc.org>
* @copyright BSD 3-Clause License
*/
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
