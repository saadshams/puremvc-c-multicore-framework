#pragma once

#include "puremvc/INotifier.h"

struct Notifier {
    struct INotifier base;

    const char *multitonKey;
};
