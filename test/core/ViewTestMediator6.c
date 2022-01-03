#include "ViewTestMediator6.h"
#include "ViewTest.h"
#include "interfaces/Notifier.h"
#include <stdlib.h>
#include <string.h>

static const char *const *listNotificationInterests(Mediator *self) {
    static const char *const interests[] = {NOTE6, NULL};
    return interests;
}

static void handleNotification(Mediator *self, Notification *notification) {
    Facade *facade = self->notifier->getFacade(self->notifier);
    Mediator *mediator = facade->removeMediator(facade, self->mediatorName);
    $Mediator.delete(mediator);
}

static void onRemove(Mediator *self) {
    ((ViewTest *) self->viewComponent)->counter++;
}

ViewTestMediator6 *NewViewTestMediator6(char *name, void *viewComponent) {
    ViewTestMediator6 *self = malloc(sizeof(ViewTestMediator6));
    $Mediator.init(&self->mediator, name, viewComponent);
    self->mediator.listNotificationInterests = listNotificationInterests;
    self->mediator.handleNotification = handleNotification;
    self->mediator.onRemove = onRemove;
    return self;
}
