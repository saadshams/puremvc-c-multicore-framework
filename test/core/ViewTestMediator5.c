#include "ViewTestMediator5.h"
#include "ViewTest.h"
#include <stdlib.h>
#include <string.h>

static char **listNotificationInterests(const Mediator *self) {
    char **list = (char *[]){NOTE5, NULL};
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
   viewTest->counter++;
}

ViewTestMediator5* NewViewTestMediator5(void *viewComponent) {
    ViewTestMediator5* self = malloc(sizeof(ViewTestMediator5));
    InitMediator(&self->mediator, ViewTestMediator5_NAME, viewComponent);
    self->mediator.listNotificationInterests = listNotificationInterests;
    self->mediator.handleNotification = handleNotification;
    return self;
}