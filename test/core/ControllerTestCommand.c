#include "ControllerTestCommand.h"
#include "ControllerTestVO.h"
#include <stdlib.h>

static void execute(const SimpleCommand *self, Notification *notification) {
    ControllerTestVO *vo = (ControllerTestVO *)notification->getBody(notification);
    vo->result = 2 * vo->input;
}

ControllerTestCommand *NewControllerTestCommand() {
    ControllerTestCommand *self = malloc(sizeof(ControllerTestCommand));
    InitSimpleCommand(&self->simpleCommand);
    self->simpleCommand.execute = execute;
    return self;
}