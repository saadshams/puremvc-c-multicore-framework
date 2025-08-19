#pragma once

#include "puremvc/puremvc.h"
#include "FacadeTestVO.c"

static void execute(const struct ICommand *self, struct INotification *notification) {
    struct FacadeTestVO *vo = (struct FacadeTestVO *)notification->getBody(notification);

    // Fabricate a result
    vo->result = 2 * vo->input;
}

struct ICommand *test_facade_command_new() {
    struct ICommand *command = puremvc_simple_command_new();
    command->execute = execute;
    return command;
}
