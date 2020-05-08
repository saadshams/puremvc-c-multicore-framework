#include "FacadeTestCommand.h"
#include "FacadeTestVO.h"
#include <stdlib.h>

void execute(const SimpleCommand *self, Notification *notification) {
    FacadeTestVO *vo = notification->getBody(notification);
    vo->result = 2 * vo->input;
}

FacadeTestCommand *NewFacadeTestCommand() {
    FacadeTestCommand *self = malloc(sizeof(FacadeTestCommand));
    InitSimpleCommand(&self->simpleCommand);
    self->simpleCommand.execute = execute;
    return self;
}
