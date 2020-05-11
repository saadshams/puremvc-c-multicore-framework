#include "ViewTestMediator6.h"
#include "ViewTest.h"
#include "interfaces/Notifier.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static const char * const *listNotificationInterests(const Mediator *self) {
    static const char * const interests[] = {NOTE6, NULL};
    return interests;
}

static void handleNotification(const Mediator *self, Notification *notification) {
    Facade *facade = self->notifier->getFacade(self->notifier);
    Mediator *mediator = facade->removeMediator(facade, self->mediatorName);
    DeleteMediator(mediator);
}

static void onRemove(Mediator *self) {
    ((ViewTest *)self->viewComponent)->counter++;
}

ViewTestMediator6* NewViewTestMediator6(char *name, void *viewComponent) {
    ViewTestMediator6* self = malloc(sizeof(ViewTestMediator6));
    InitMediator(&self->mediator, name, viewComponent);
    self->mediator.listNotificationInterests = listNotificationInterests;
    self->mediator.handleNotification = handleNotification;
    self->mediator.onRemove = onRemove;
    return self;
}
