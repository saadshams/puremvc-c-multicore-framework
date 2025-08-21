#include "SimpleCommandTestCommand.h"
#include "SimpleCommandTestVO.h"

/**
 * Fabricate a result by multiplying the input by 2
 *
 * @param self
 * @param notification the <code>INotification</code> carrying the <code>SimpleCommandTestVO</code>
 */
static void execute(const struct ICommand *self, struct INotification *notification) {
    struct SimpleCommandTestVO *vo = (struct SimpleCommandTestVO *) notification->getBody(notification);

    // Fabricate a result
    vo->result = 2 * vo->input;
}

struct ICommand *test_simple_command_new() {
    struct ICommand *command = puremvc_simple_command_new();
    command->execute = execute;
    return command;
}
