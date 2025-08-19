#pragma once

#include "INotification.h"
#include "INotifier.h"

struct ICommand {
    struct INotifier *notifier;

    void (*execute)(const struct ICommand *self, struct INotification *notification);
};

struct ICommand *puremvc_simple_command_new();
void puremvc_simple_command_free(struct ICommand **command);
