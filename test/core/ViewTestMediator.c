#include "ViewTestMediator.h"
#include <stdlib.h>

// be sure that the mediator has some Observers created
// in order to test removeMediator
static const char * const *listNotificationInterests(Mediator *self) {
    static const char * const interests[] = {"ABC", "DEF", "GHI", NULL};
    return interests;
}

ViewTestMediator *NewViewTestMediator(void *viewComponent) {
    ViewTestMediator *self = malloc(sizeof(ViewTestMediator));
    $Mediator.init(&self->mediator, ViewTestMediator_NAME, viewComponent);
    self->mediator.listNotificationInterests = listNotificationInterests;
    return self;
}