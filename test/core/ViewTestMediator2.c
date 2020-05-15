#include "ViewTestMediator2.h"
#include "ViewTest.h"
#include <stdlib.h>
#include <string.h>

static const char * const *listNotificationInterests(Mediator *self) {
    // be sure that the mediator has some Observers created
    // in order to test removeMediator
    static const char * const interests[] = {NOTE1, NOTE2, NULL};
    return interests;
}

static void handleNotification(Mediator *self, Notification *notification) {
    ViewTest *viewTest = self->getViewComponent(self);
    viewTest->lastNotification = notification->name;
}

ViewTestMediator2 *NewViewTestMediator2(void *viewComponent) {
    ViewTestMediator2 *self = malloc(sizeof(ViewTestMediator2));
    InitMediator(&self->mediator, ViewTestMediator2_NAME, viewComponent);
    self->mediator.listNotificationInterests = listNotificationInterests;
    self->mediator.handleNotification = handleNotification;
    return self;
}