/**
* @file Facade.h
* @internal
* @brief Facade Header
*
* @author Saad Shams <saad.shams@puremvc.org>
* @copyright BSD 3-Clause License
*/
#pragma once

#include "puremvc/IFacade.h"

struct Facade {
    struct IFacade base;

    const char *multitonKey;
    struct IController *controller;
    struct IModel *model;
    struct IView *view;
};
