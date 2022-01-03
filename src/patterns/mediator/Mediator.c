#include "interfaces/Mediator.h"
#include "interfaces/Notifier.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char *getMediatorName(Mediator *self) {
    return self->mediatorName;
}

static void setViewComponent(Mediator *self, void *viewComponent) {
    self->viewComponent = viewComponent;
}

static void *getViewComponent(Mediator *self) {
    return self->viewComponent;
}

static const char *const *listNotificationInterests(Mediator *self) {
    static const char *const interests[] = {NULL};
    return interests;
}

static void handleNotification(Mediator *self, Notification *notification) {

}

static void onRegister(Mediator *self) {

}

static void onRemove(Mediator *self) {

}

static void init(Mediator *mediator, const char *mediatorName, void *viewComponent) {
    mediator->notifier = $Notifier.new();
    mediator->mediatorName = mediatorName != NULL ? mediatorName : MEDIATOR_NAME;
    mediator->viewComponent = viewComponent;
    mediator->getMediatorName = getMediatorName;
    mediator->setViewComponent = setViewComponent;
    mediator->getViewComponent = getViewComponent;
    mediator->listNotificationInterests = listNotificationInterests;
    mediator->handleNotification = handleNotification;
    mediator->onRegister = onRegister;
    mediator->onRemove = onRemove;
}

static Mediator *new(const char *mediatorName, void *viewComponent) {
    Mediator *mediator = malloc(sizeof(Mediator));
    if (mediator == NULL) goto exception;
    init(mediator, mediatorName, viewComponent);
    return mediator;

    exception:
    fprintf(stderr, "Mediator allocation failed.\n");
    return NULL;
}

static void delete(Mediator *mediator) {
    $Notifier.delete(mediator->notifier);
    free(mediator);
    mediator = NULL;
}

const struct $Mediator $Mediator = {.new = new, .init = init, .delete = delete};
