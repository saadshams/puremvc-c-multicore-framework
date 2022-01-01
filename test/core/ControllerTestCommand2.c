#include "ControllerTestCommand2.h"
#include "ControllerTestVO.h"

/**
 * Fabricate a result by multiplying the input by 2 and adding to the existing result
 *
 * <P>This tests accumulation effect that would show if the command were executed more than once.</P>
 *
 * @param self the simple command
 * @param notification the note carrying the ControllerTestVO
 */
static void execute(SimpleCommand *self, Notification *notification) {
    ControllerTestVO *vo = notification->getBody(notification);

    // Fabricate a result
    vo->result = vo->result + (2 * vo->input);
}

/**
 * Constructor
 * @return
 */
SimpleCommand *NewControllerTestCommand2() {
    SimpleCommand *self = $SimpleCommand.new();
    self->execute = execute;
    return self;
}