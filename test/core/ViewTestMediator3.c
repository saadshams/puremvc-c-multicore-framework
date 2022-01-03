#include "ViewTestMediator3.h"
#include "ViewTest.h"
#include <stdlib.h>
#include <string.h>

static const char *const *listNotificationInterests(Mediator *self) {
    // be sure that the mediator has some Observers created
    // in order to test removeMediator
    static const char *const interests[] = {NOTE3, NULL};
    return interests;
}

static void handleNotification(Mediator *self, Notification *notification) {
    ViewTest *viewTest = self->getViewComponent(self);
    viewTest->lastNotification = notification->name;
}

ViewTestMediator3 *NewViewTestMediator3(void *viewComponent) {
    ViewTestMediator3 *self = malloc(sizeof(ViewTestMediator3));
    $Mediator.init(&self->mediator, ViewTestMediator3_NAME, viewComponent);
    self->mediator.listNotificationInterests = listNotificationInterests;
    self->mediator.handleNotification = handleNotification;
    return self;
}