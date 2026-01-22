/**
* @file Mediator.h
* @internal
* @brief Mediator Header
*
* @author Saad Shams <saad.shams@puremvc.org>
* @copyright BSD 3-Clause License
*/
#pragma once

#include "puremvc/IMediator.h"

#define MEDIATOR_NAME "mediator"

struct Mediator {
    struct IMediator base;

    const char *name;
    void *component;
};
