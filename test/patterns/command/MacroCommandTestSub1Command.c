#include "MacroCommandTestSub1Command.h"
#include "MacroCommandTestVO.h"

static void execute(const struct ICommand *self, struct INotification *notification, const char **error) {
    struct MacroCommandTestVO *vo = (struct MacroCommandTestVO *)notification->getBody(notification);

    // Fabricate a result
    vo->result1 = 2 * vo->input;
}

struct ICommand *macro_command_test_sub1command_new(const char **error) {
    struct ICommand *command = puremvc_simple_command_new(error);
    command->execute = execute;
    return command;
}
