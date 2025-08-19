#pragma once

#include "puremvc/INotification.h"

struct Notification {
    struct INotification base;

    const char *name;
    void *body;
    char *type;
};
