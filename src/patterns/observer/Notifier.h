/**
* @file Notifier.h
* @internal
* @brief Notifier Header
*
* @author Saad Shams <saad.shams@puremvc.org>
* @copyright BSD 3-Clause License
*/
#pragma once

#include "puremvc/INotifier.h"

struct Notifier {
    struct INotifier base;

    const char *key;
};
