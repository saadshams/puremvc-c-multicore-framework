#include "interfaces/Mediator.h"
#include "interfaces/Notifier.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *getMediatorName(const Mediator *self) {
    return self->mediatorName;
}

static void setViewComponent(Mediator *self, void *viewComponent) {
    self->viewComponent = viewComponent;
}

static void *getViewComponent(const Mediator *self) {
    return self->viewComponent;
}

static const char * const *listNotificationInterests(const Mediator *self) {
    static const char * const interests[] = {NULL};
    return interests;
}

static void handleNotification(const Mediator *self, Notification *notification) {

}

static void onRegister(Mediator *self) {

}

static void onRemove(Mediator *self) {

}

void InitMediator(Mediator *self, char *mediatorName, void *viewComponent) {
    self->notifier = NewNotifier();
    self->mediatorName = strdup(mediatorName != NULL ? mediatorName : MEDIATOR_NAME);
    self->viewComponent = viewComponent;
    self->getMediatorName = getMediatorName;
    self->setViewComponent = setViewComponent;
    self->getViewComponent = getViewComponent;
    self->listNotificationInterests = listNotificationInterests;
    self->handleNotification = handleNotification;
    self->onRegister = onRegister;
    self->onRemove = onRemove;
}

Mediator *NewMediator(char *mediatorName, void *viewComponent) {
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
    free(self->mediatorName);
    free(self);
    self = NULL;
}
