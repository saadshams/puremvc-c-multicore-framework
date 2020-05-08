#include "interfaces/Mediator.h"
#include "interfaces/Notifier.h"
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

static char **listNotificationInterests(const Mediator *self) {
    char **list = (char *[]){NULL};
    char **interests = malloc(sizeof(char*) * 1);
    int i = 0;
    while(*list) {
        interests[i] = malloc(sizeof(char) * (strlen(*list) + 1));
        strcpy(interests[i++], *list);
        list++;
    }
    interests[i] = NULL;
    return interests;
}

static void handleNotification(const Mediator *self, Notification *notification) {

}

static void onRegister(Mediator *self) {

}

static void onRemove(Mediator *self) {

}

void InitMediator(Mediator *self, char *mediatorName, void *viewComponent) {
    if (self) {
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
}

Mediator *NewMediator(char *mediatorName, void *viewComponent) {
    Mediator *self = malloc(sizeof(Mediator));
    InitMediator(self, mediatorName, viewComponent);
    return self;
}

void DeleteMediator(Mediator *self) {
    DeleteNotifier(self->notifier);
    free(self->mediatorName);
    free(self);
}
