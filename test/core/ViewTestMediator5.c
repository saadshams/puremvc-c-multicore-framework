#include <stdlib.h>

#include "ViewTestMediator5.h"

static char **listNotificationInterests(const struct IMediator *self) {
    static const char* interests[] = {NOTE5, NULL};
    return self->allocNotificationInterests(self, interests);
}

static void handleNotification(const struct IMediator *self, struct INotification *notification) {
    struct ViewTest *viewTest = self->getComponent(self);
    viewTest->counter++;
}

struct IMediator *test_mediator5_new(struct ViewTest *component) {
    struct IMediator *mediator = puremvc_mediator_new(ViewTestMediator5_NAME, component);
    mediator->listNotificationInterests = listNotificationInterests;
    mediator->handleNotification = handleNotification;
    return mediator;
}
