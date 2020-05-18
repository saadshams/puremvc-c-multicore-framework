#include "MacroCommandTestSub2Command.h"
#include "MacroCommandTestVO.h"

/**
 * Fabricate a result by multiplying the input by itself
 *
 * @param notification the <code>IEvent</code> carrying the <code>MacroCommandTestVO</code>
 */
static void execute(SimpleCommand *self, Notification *notification) {
    MacroCommandTestVO *vo = notification->getBody(notification);

    // Fabricate a result
    vo->result2 = vo->input * vo->input;
}

/**
 * A SimpleCommand struct used by MacroCommandTestCommand.
 *
 * @see MacroCommandTest
 * @see MacroCommandTestCommand
 * @see MacroCommandTestVO
 */
SimpleCommand *NewMacroCommandTestSub2Command() {
    SimpleCommand *self = NewSimpleCommand();
    self->execute = execute;
    return self;
}
