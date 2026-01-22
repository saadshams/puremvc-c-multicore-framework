#include <stdio.h>

#include "ViewTestMediator6.h"

static char **listNotificationInterests(const struct IMediator *self) {
    static const char *interests[] = { NOTE6, NULL };
    return self->allocNotificationInterests(self, interests);
}

static void handleNotification(const struct IMediator *self, struct INotification *notification) {
    const char *error = NULL;
    const struct IFacade *facade = self->notifier->getFacade(self->notifier, &error);
    struct IMediator *mediator = facade->removeMediator(facade, self->getName(self));
    puremvc_mediator_free(&mediator);
}

static void onRemove(struct IMediator *self) {
    ((struct ViewTest *) self->getComponent(self))->counter++;
}

struct IMediator *test_mediator6_new(const char *name, struct ViewTest *component) {
    const char *error = NULL;
    struct IMediator *mediator = puremvc_mediator_new(name, component, &error);
    mediator->listNotificationInterests = listNotificationInterests;
    mediator->handleNotification = handleNotification;
    mediator->onRemove = onRemove;
    return mediator;
}
