#include "SimpleCommandTestCommand.h"
#include "SimpleCommandTestVO.h"

static void execute(const SimpleCommand *self, Notification *notification) {
    SimpleCommandTestVO *vo = notification->getBody(notification);
    vo->result = 2 * vo->input;
}

SimpleCommand* NewSimpleCommandTestCommand() {
    SimpleCommand *self = NewSimpleCommand();
    self->execute = execute;
    return self;
}