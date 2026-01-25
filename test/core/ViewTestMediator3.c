#include <stdlib.h>

#include "ViewTestMediator3.h"

static const char **listNotificationInterests(const struct IMediator *self, const char **error) {
    static const char* interests[] = {NOTE3, NULL};
    return interests;
}

static void handleNotification(const struct IMediator *self, struct INotification *notification) {
    struct ViewTest *viewTest = self->getComponent(self);
    viewTest->lastNotification = notification->getName(notification);
}

struct IMediator *test_mediator3_new(struct ViewTest *component) {
    const char *error = NULL;
    struct IMediator *mediator = puremvc_mediator_new(ViewTestMediator3_NAME, component, &error);
    mediator->listNotificationInterests = listNotificationInterests;
    mediator->handleNotification = handleNotification;
    return mediator;
}
