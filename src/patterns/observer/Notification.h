/**
* @file Notification.h
* @internal
* @brief Notification Header
*
* @author Saad Shams <saad.shams@puremvc.org>
* @copyright BSD 3-Clause License
*/
#pragma once

#include "puremvc/INotification.h"

struct Notification {
    struct INotification base;

    const char *name;
    void *body;
    char *type;
};
