#include "ControllerTestCommand2.h"
#include "ControllerTestVO.h"
#include <stdlib.h>

static void execute(const ControllerTestCommand2 *self, Notification *notification) {
    ControllerTestVO *vo = notification->getBody(notification);

    vo->result = vo->result + (2 * vo->input);
}

ControllerTestCommand2 *NewControllerTestCommand2() {
    ControllerTestCommand2 *self = malloc(sizeof(ControllerTestCommand2));
    InitSimpleCommand(&self->simpleCommand);
    self->simpleCommand.execute = (void (*)(const SimpleCommand *, Notification *)) execute;
    return self;
}