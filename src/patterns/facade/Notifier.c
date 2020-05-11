#include "interfaces/Notifier.h"
#include <stdio.h>
#include <string.h>

Facade *getFacade(Notifier *self) {
    return getFacadeInstance(self->multitonKey, NewFacade);
}

static void sendNotification(Notifier *self, const char *notificationName, void *body, char *type) {
    Facade *facade = self->getFacade(self);
    facade->sendNotification(facade, notificationName, body, type);
}

static void initializeNotifier(Notifier *self, char *key) {
    self->multitonKey = strdup(key);
}

void InitNotifier(Notifier *self) {
    self->multitonKey = NULL;
    self->getFacade = getFacade;
    self->initializeNotifier  = initializeNotifier;
    self->sendNotification = sendNotification;
}

Notifier *NewNotifier() {
    Notifier *self = malloc(sizeof(Notifier));
    if (self == NULL) goto exception;
    InitNotifier(self);
    return self;

    exception:
    fprintf(stderr, "Notifier allocation failed.\n");
    return NULL;
}

void DeleteNotifier(Notifier *self) {
    if (self->multitonKey) free(self->multitonKey);
    free(self);
    self = NULL;
}
