#pragma once

#include "puremvc/IObserver.h"

struct Observer {
    struct IObserver base;

    void *context;
    void (*notify)(const void *context, struct INotification *notification);
};
