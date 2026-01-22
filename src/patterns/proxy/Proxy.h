/**
* @file Proxy.h
* @internal
* @brief Proxy Header
*
* @author Saad Shams <saad.shams@puremvc.org>
* @copyright BSD 3-Clause License
*/
#pragma once

#include "puremvc/IProxy.h"

struct Proxy {
    struct IProxy base;

    const char* name;
    void* data;
};
