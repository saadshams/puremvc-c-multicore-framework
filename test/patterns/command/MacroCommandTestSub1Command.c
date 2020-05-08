#include "MacroCommandTestSub1Command.h"
#include "MacroCommandTestVO.h"

static void execute(const SimpleCommand *self, Notification *notification) {
    MacroCommandTestVO *vo = notification->getBody(notification);
    vo->result1 = 2 * vo->input;
}

SimpleCommand* MacroCommandTestSub1CommandNew() {
    SimpleCommand *self = NewSimpleCommand();
    self->execute = execute;
    return self;
}