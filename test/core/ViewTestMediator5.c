#pragma once

#include <stdlib.h>

#include "puremvc/puremvc.h"
#include "ViewTestComponent.c"

#define ViewTestMediator5_NAME "Mediator5"

static char **listNotificationInterests5(const struct IMediator *self) {
    static const char* interests[] = {NOTE5, NULL};
    return self->allocNotificationInterests(self, interests);
}

static void handleNotification5(const struct IMediator *self, struct INotification *notification) {
    struct ViewTest *viewTest = self->getComponent(self);
    viewTest->counter++;
}

struct IMediator *test_mediator5_new(struct ViewTest *component) {
    struct IMediator *mediator = puremvc_mediator_new(ViewTestMediator5_NAME, component);
    mediator->listNotificationInterests = listNotificationInterests5;
    mediator->handleNotification = handleNotification5;
    return mediator;
}
