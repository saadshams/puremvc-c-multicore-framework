#pragma once

#include "ICommand.h"

struct IMacroCommand {
    struct ICommand command;

    void (*initializeMacroCommand)(const struct IMacroCommand *self);
    void (*addSubCommand)(const struct IMacroCommand *self, struct ICommand *(*factory)());
};

struct IMacroCommand *puremvc_macro_command_new();
void puremvc_macro_command_free(struct IMacroCommand **command);
