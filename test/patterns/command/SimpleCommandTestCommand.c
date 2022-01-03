#include "SimpleCommandTestCommand.h"
#include "SimpleCommandTestVO.h"
#include <stdlib.h>

/**
 * Fabricate a result by multiplying the input by 2
 *
 * @param notification the <code>INotification</code> carrying the <code>SimpleCommandTestVO</code>
 */
static void execute(SimpleCommand *self, Notification *notification) {
    SimpleCommandTestVO *vo = notification->getBody(notification);

    // Fabricate a result
    vo->result = 2 * vo->input;
}

/**
 * Constructor
 */
SimpleCommandTestCommand *NewSimpleCommandTestCommand() {
    SimpleCommandTestCommand *self = malloc(sizeof(SimpleCommandTestCommand));
    $SimpleCommand.init(&self->simpleCommand);
    self->simpleCommand.execute = execute; // override implementation
    return self;
}

/**
 * Destructor
 */
void DeleteSimpleCommandTestCommand(SimpleCommandTestCommand *self) {
    $SimpleCommand.delete(&self->simpleCommand);
}
