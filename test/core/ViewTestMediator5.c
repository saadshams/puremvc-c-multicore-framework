#include "ViewTestMediator5.h"
#include "ViewTest.h"
#include <stdlib.h>
#include <string.h>

static const char * const *listNotificationInterests(Mediator *self) {
    static const char * const interests[] = {NOTE5, NULL};
    return interests;
}

static void handleNotification(Mediator *self, Notification *notification) {
    ViewTest *viewTest = self->getViewComponent(self);
   viewTest->counter++;
}

ViewTestMediator5* NewViewTestMediator5(void *viewComponent) {
    ViewTestMediator5* self = malloc(sizeof(ViewTestMediator5));
    InitMediator(&self->mediator, ViewTestMediator5_NAME, viewComponent);
    self->mediator.listNotificationInterests = listNotificationInterests;
    self->mediator.handleNotification = handleNotification;
    return self;
}