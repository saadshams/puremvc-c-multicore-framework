#include "ViewTestMediator6.h"
#include "ViewTest.h"
#include "interfaces/Notifier.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static char **listNotificationInterests(const Mediator *self) {
    char **list = (char *[]){NOTE6, NULL};
    char **cursor = list;
    int size = 0;
    while (*cursor) {
        size++;
        cursor++;
    }
    char **interests = malloc(sizeof(char*) * size + 1);
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
