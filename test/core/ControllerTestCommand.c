#include "ControllerTestCommand.h"
#include "ControllerTestVO.h"

/**
 * Fabricate a result by multiplying the input by 2
 *
 * @param notification the note carrying the ControllerTestVO
 */
static void execute(SimpleCommand *self, Notification *notification) {
    ControllerTestVO *vo = (ControllerTestVO *) notification->getBody(notification);

    // Fabricate a result
    vo->result = 2 * vo->input;
}

SimpleCommand *NewControllerTestCommand() {
    SimpleCommand *self = $SimpleCommand.new();
    self->execute = execute;
    return self;
}
