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

static const char * const *listNotificationInterests(Mediator *self) {
    static const char * const interests[] = {NULL};
    return interests;
}

static void handleNotification(Mediator *self, Notification *notification) {

}

static void onRegister(Mediator *self) {

}

static void onRemove(Mediator *self) {

}

void InitMediator(Mediator *self, const char *mediatorName, void *viewComponent) {
    self->notifier = NewNotifier();
    self->mediatorName = mediatorName != NULL ? mediatorName : MEDIATOR_NAME;
    self->viewComponent = viewComponent;
    self->getMediatorName = getMediatorName;
    self->setViewComponent = setViewComponent;
    self->getViewComponent = getViewComponent;
    self->listNotificationInterests = listNotificationInterests;
    self->handleNotification = handleNotification;
    self->onRegister = onRegister;
    self->onRemove = onRemove;
}

Mediator *NewMediator(const char *mediatorName, void *viewComponent) {
    Mediator *self = malloc(sizeof(Mediator));
    if (self == NULL) goto exception;
    InitMediator(self, mediatorName, viewComponent);
    return self;

    exception:
        fprintf(stderr, "Mediator allocation failed.\n");
        return NULL;
}

void DeleteMediator(Mediator *self) {
    DeleteNotifier(self->notifier);
    free(self);
    self = NULL;
}
