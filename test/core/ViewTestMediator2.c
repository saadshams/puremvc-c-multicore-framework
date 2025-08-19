#pragma once

#include <stdlib.h>

#include "puremvc/puremvc.h"
#include "ViewTestComponent.c"

#define ViewTestMediator2_NAME "Mediator2"

static char **listNotificationInterests2(const struct IMediator *self) {
    static const char* interests[] = {NOTE1, NOTE2, NULL};
    return self->allocNotificationInterests(self, interests);
}

static void handleNotification(const struct IMediator *self, struct INotification *notification) {
    struct ViewTest *viewTest = self->getComponent(self);
    viewTest->lastNotification = notification->getName(notification);
}

struct IMediator *test_mediator2_new(struct ViewTest *component) {
    struct IMediator *mediator = puremvc_mediator_new(ViewTestMediator2_NAME, component);
    mediator->listNotificationInterests = listNotificationInterests2;
    mediator->handleNotification = handleNotification;
    return mediator;
}
