#pragma once

#include "puremvc/IMediator.h"

#define MEDIATOR_NAME "mediator"

struct Mediator {
    struct IMediator base;

    const char *name;
    void *component;
};
