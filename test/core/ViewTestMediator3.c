#pragma once

#include <stdlib.h>

#include "puremvc/puremvc.h"
#include "ViewTestComponent.c"

#define ViewTestMediator3_NAME "Mediator3"

static char **listNotificationInterests3(const struct IMediator *self) {
    static const char* interests[] = {NOTE3, NULL};
    return self->allocNotificationInterests(self, interests);
}

static void handleNotification3(const struct IMediator *self, struct INotification *notification) {
    struct ViewTest *viewTest = self->getComponent(self);
    viewTest->lastNotification = notification->getName(notification);
}

struct IMediator *test_mediator3_new(struct ViewTest *component) {
    struct IMediator *mediator = puremvc_mediator_new(ViewTestMediator3_NAME, component);
    mediator->listNotificationInterests = listNotificationInterests3;
    mediator->handleNotification = handleNotification3;
    return mediator;
}
