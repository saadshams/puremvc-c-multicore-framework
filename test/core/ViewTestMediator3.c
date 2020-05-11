#include "ViewTestMediator3.h"
#include "ViewTest.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static const char * const *listNotificationInterests(const Mediator *self) {
    static const char * const interests[] = {NOTE3, NULL};
    return interests;
}

static void handleNotification(const Mediator *self, Notification *notification) {
    ViewTest *viewTest = self->getViewComponent(self);
    viewTest->lastNotification = strdup(notification->name);
}

ViewTestMediator3* NewViewTestMediator3(void *viewComponent) {
    ViewTestMediator3* self = malloc(sizeof(ViewTestMediator3));
    InitMediator(&self->mediator, ViewTestMediator3_NAME, viewComponent);
    self->mediator.listNotificationInterests = listNotificationInterests;
    self->mediator.handleNotification = handleNotification;
    return self;
}