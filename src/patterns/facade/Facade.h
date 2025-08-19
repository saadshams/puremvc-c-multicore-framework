#pragma once

#include "puremvc/IFacade.h"

struct Facade {
    struct IFacade base;

    const char *multitonKey;
    struct IController *controller;
    struct IModel *model;
    struct IView *view;
};
