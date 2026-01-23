#include <stdlib.h>

#include "ViewTestMediator2.h"

static char **listNotificationInterests(const struct IMediator *self, const char **error) {
    static const char* interests[] = {NOTE1, NOTE2, NULL};
    return self->allocNotificationInterests(self, interests, error);
}

static void handleNotification(const struct IMediator *self, struct INotification *notification) {
    struct ViewTest *viewTest = self->getComponent(self);
    viewTest->lastNotification = notification->getName(notification);
}

struct IMediator *test_mediator2_new(struct ViewTest *component) {
    const char *error = NULL;
    struct IMediator *mediator = puremvc_mediator_new(ViewTestMediator2_NAME, component, &error);
    mediator->listNotificationInterests = listNotificationInterests;
    mediator->handleNotification = handleNotification;
    return mediator;
}
