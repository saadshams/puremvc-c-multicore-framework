#include "ViewTestMediator3.h"
#include "ViewTest.h"
#include <stdlib.h>
#include <string.h>

static char **listNotificationInterests(const Mediator *self) {
    char **list = (char *[]){NOTE3, NULL};
    char **interests = malloc(sizeof(char*) * 2);
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
    ViewTest *viewTest = self->getViewComponent(self);
    viewTest->lastNotification = notification->getName(notification);
}

ViewTestMediator3* NewViewTestMediator3(void *viewComponent) {
    ViewTestMediator3* self = malloc(sizeof(ViewTestMediator3));
    InitMediator(&self->mediator, ViewTestMediator3_NAME, viewComponent);
    self->mediator.listNotificationInterests = listNotificationInterests;
    self->mediator.handleNotification = handleNotification;
    return self;
}