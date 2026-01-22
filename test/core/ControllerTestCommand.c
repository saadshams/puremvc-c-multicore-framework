#include <stdlib.h>

#include "ControllerTestCommand.h"
#include "ControllerTestVO.h"

static void execute(const struct ICommand *self, struct INotification *notification, const char **error) {
    struct ControllerTestVO *vo = (struct ControllerTestVO *) notification->getBody(notification);

    // Fabricate a result
    vo->result = 2 * vo->input;
}

struct ICommand *test_controller_command_new(const char **error) {
    struct ICommand *command = puremvc_simple_command_new(error);
    command->execute = execute;
    return command;
}
