#pragma once

#include <stdbool.h>

#include "ICommand.h"

struct IController {
    void (*initializeController)(struct IController *self);

    void (*registerCommand)(const struct IController *self, const char *notificationName, struct ICommand *(*factory)());
    void (*executeCommand)(const struct IController *self, struct INotification *notification);
    bool (*hasCommand)(const struct IController *self, const char *notificationName);
    void (*removeCommand)(const struct IController *self, const char *notificationName);
};

struct IController *puremvc_controller_new(const char *key);
void puremvc_controller_free(struct IController **controller);

struct IController *puremvc_controller_getInstance(const char *key, struct IController *(*factory)(const char *));
void puremvc_controller_removeController(const char *key);
