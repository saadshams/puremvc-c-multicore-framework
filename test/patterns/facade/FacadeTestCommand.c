#include "FacadeTestCommand.h"
#include "FacadeTestVO.h"
#include <stdlib.h>

/**
 * Fabricate a result by multiplying the input by 2
 *
 * @param notification the Notification carrying the FacadeTestVO
 */
void execute(SimpleCommand *self, Notification *notification) {
    FacadeTestVO *vo = notification->getBody(notification);

    // Fabricate a result
    vo->result = 2 * vo->input;
}

FacadeTestCommand *NewFacadeTestCommand() {
    FacadeTestCommand *self = malloc(sizeof(FacadeTestCommand));
    InitSimpleCommand(&self->simpleCommand);
    self->simpleCommand.execute = execute;
    return self;
}
