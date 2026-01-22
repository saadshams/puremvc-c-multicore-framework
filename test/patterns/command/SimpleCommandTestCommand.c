#include <stdlib.h>

#include "SimpleCommandTestCommand.h"
#include "SimpleCommandTestVO.h"

/**
 * Fabricate a result by multiplying the input by 2
 *
 * @param self
 * @param notification the <code>INotification</code> carrying the <code>SimpleCommandTestVO</code>
 */
static void execute(const struct ICommand *self, struct INotification *notification, const char **error) {
    struct SimpleCommandTestVO *vo = (struct SimpleCommandTestVO *) notification->getBody(notification);

    // Fabricate a result
    vo->result = 2 * vo->input;
}

struct ICommand *test_simple_command_new() {
    const char *error = NULL;
    struct ICommand *command = puremvc_simple_command_new(&error);
    command->execute = execute;
    return command;
}
