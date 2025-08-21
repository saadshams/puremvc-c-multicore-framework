#include <stdlib.h>

#include "puremvc/IMediator.h"
#include "ViewTestMediator3.h"

static char **listNotificationInterests(const struct IMediator *self) {
    static const char* interests[] = {NOTE3, NULL};
    return self->allocNotificationInterests(self, interests);
}

static void handleNotification(const struct IMediator *self, struct INotification *notification) {
    struct ViewTest *viewTest = self->getComponent(self);
    viewTest->lastNotification = notification->getName(notification);
}

struct IMediator *test_mediator3_new(struct ViewTest *component) {
    struct IMediator *mediator = puremvc_mediator_new(ViewTestMediator3_NAME, component);
    mediator->listNotificationInterests = listNotificationInterests;
    mediator->handleNotification = handleNotification;
    return mediator;
}
