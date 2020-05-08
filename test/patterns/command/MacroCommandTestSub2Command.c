#include "MacroCommandTestSub2Command.h"
#include "MacroCommandTestVO.h"

static void execute(const SimpleCommand *self, Notification *notification) {
    MacroCommandTestVO *vo = notification->getBody(notification);
    vo->result2 = vo->input * vo->input;
}

SimpleCommand *MacroCommandTestSub2CommandNew() {
    SimpleCommand *self = NewSimpleCommand();
    self->execute = execute;
    return self;
}