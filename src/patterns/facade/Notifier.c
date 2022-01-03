#include "interfaces/Notifier.h"
#include <stdio.h>
#include <string.h>

static Facade *getFacade(Notifier *self) {
    return $Facade.getInstance(self->multitonKey, $Facade.new);
}

static void sendNotification(Notifier *self, const char *notificationName, void *body, char *type) {
    Facade *facade = self->getFacade(self);
    facade->sendNotification(facade, notificationName, body, type);
}

static void initializeNotifier(Notifier *self, const char *key) {
    self->multitonKey = key;
}

static void init(Notifier *notifier) {
    notifier->multitonKey = NULL;
    notifier->getFacade = getFacade;
    notifier->initializeNotifier = initializeNotifier;
    notifier->sendNotification = sendNotification;
}

static Notifier *new(void) {
    Notifier *notifier = malloc(sizeof(Notifier));
    if (notifier == NULL) goto exception;
    init(notifier);
    return notifier;

    exception:
    fprintf(stderr, "Notifier allocation failed.\n");
    return NULL;
}

static void delete(Notifier *notifier) {
    free(notifier);
    notifier = NULL;
}

const struct $Notifier $Notifier = {.new = new, .init = init, .delete = delete};
