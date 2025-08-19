#pragma once

#include <stdio.h>

#include "puremvc/puremvc.h"
#include "ViewTestComponent.c"

#define ViewTestMediator6_NAME "Mediator6"

static char **listNotificationInterests6(const struct IMediator *self) {
    static const char *interests[] = { NOTE6, NULL };
    return self->allocNotificationInterests(self, interests);
}

static void handleNotification6(const struct IMediator *self, struct INotification *notification) {
    const struct IFacade *facade = self->notifier->getFacade(self->notifier);
    struct IMediator *mediator = facade->removeMediator(facade, self->getName(self));
    puremvc_mediator_free(&mediator);
}

static void onRemove6(struct IMediator *self) {
    ((struct ViewTest *) self->getComponent(self))->counter++;
}

struct IMediator *test_mediator6_new(const char *name, struct ViewTest *component) {
    struct IMediator *mediator = puremvc_mediator_new(name, component);
    mediator->listNotificationInterests = listNotificationInterests6;
    mediator->handleNotification = handleNotification6;
    mediator->onRemove = onRemove6;
    return mediator;
}
