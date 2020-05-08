#include "ViewTestMediator.h"
#include <stdlib.h>

ViewTestMediator *NewViewTestMediator(void *viewComponent) {
    ViewTestMediator *self = malloc(sizeof(ViewTestMediator));
    InitMediator(&self->mediator, ViewTestMediator_NAME, viewComponent);
    return self;
}