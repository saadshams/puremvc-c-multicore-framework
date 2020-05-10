#include "ViewTestMediator2.h"
#include "ViewTest.h"
#include <stdlib.h>
#include <string.h>

static char **listNotificationInterests(const Mediator *self) {
    char **list = (char *[]){NOTE1, NOTE2, NULL};

    char **cursor = list;
    int size = 0;
    while (*cursor) {
        size++;
        cursor++;
    }
    char **interests = malloc(sizeof(char*) * (size + 1));
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
    viewTest->lastNotification = strdup(notification->name);
}

ViewTestMediator2 *ViewTestMediator2New(void *viewComponent) {
    ViewTestMediator2 *self = malloc(sizeof(ViewTestMediator2));
    InitMediator(&self->mediator, ViewTestMediator2_NAME, viewComponent);
    self->mediator.listNotificationInterests = listNotificationInterests;
    self->mediator.handleNotification = handleNotification;
    return self;
}