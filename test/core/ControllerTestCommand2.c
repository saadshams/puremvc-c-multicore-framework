#include "ControllerTestCommand2.h"
#include "ControllerTestVO.h"

static void controller_execute(const struct ICommand *self, struct INotification *notification) {
    struct ControllerTestVO *vo = (struct ControllerTestVO *)notification->getBody(notification);

    // Fabricate a result
    vo->result = vo->result + (2 * vo->input);
}

struct ICommand *test_controller_command2_new() {
    struct ICommand *command = puremvc_simple_command_new();
    command->execute = controller_execute;
    return command;
}
