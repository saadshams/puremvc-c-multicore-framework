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

static void initializeNotifier(Notifier *self, const char *key) {
    self->multitonKey = key;
}

Notifier *NewNotifier(void) {
    Notifier *notifier = malloc(sizeof(Notifier));
    if (notifier == NULL) goto exception;
    InitNotifier(notifier);
    return notifier;

    exception:
        fprintf(stderr, "Notifier allocation failed.\n");
        return NULL;
}

void InitNotifier(Notifier *notifier) {
    notifier->multitonKey = NULL;
    notifier->getFacade = getFacade;
    notifier->initializeNotifier  = initializeNotifier;
    notifier->sendNotification = sendNotification;
}

void DeleteNotifier(Notifier *notifier) {
    free(notifier);
    notifier = NULL;
}
