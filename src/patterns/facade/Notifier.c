#include "interfaces/Notifier.h"
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
    if (self != NULL) {
        self->getFacade = getFacade;
        self->initializeNotifier  = initializeNotifier;
        self->sendNotification = sendNotification;
    }
}

Notifier *NewNotifier() {
    Notifier *self = malloc(sizeof(Notifier));
    InitNotifier(self);
    return self;
}

void DeleteNotifier(Notifier *self) {
    free(self->multitonKey);
    free(self);
}
