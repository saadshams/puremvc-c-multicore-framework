#include "ViewTestMediator4.h"
#include <stdlib.h>

void onRegister(Mediator *self) {
    ViewTest *viewTest = (ViewTest *)self->getViewComponent(self);
    viewTest->onRegisterCalled = true;
}

void onRemove(Mediator *self) {
    ViewTest *viewTest = (ViewTest *)self->getViewComponent(self);
    viewTest->onRemoveCalled = true;
}

ViewTestMediator4 *NewTestMediator4(ViewTest *view) {
    ViewTestMediator4 *self = malloc(sizeof(ViewTestMediator4));
    InitMediator(&self->mediator, ViewTestMediator4_NAME, view);
    self->mediator.onRegister = onRegister;
    self->mediator.onRemove = onRemove;
    return self;
}