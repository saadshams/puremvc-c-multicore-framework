#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "Notifier.h"
#include "puremvc/IFacade.h"

static struct IFacade *getFacade(const struct INotifier *self) {
    const struct Notifier *this = (struct Notifier *) self;
    return puremvc_facade_getInstance(this->multitonKey, puremvc_facade_new);
}

const char *getMultitonKey(const struct INotifier *self) {
    const struct Notifier *this = (struct Notifier *) self;
    return this->multitonKey;
}

static void initializeNotifier(struct INotifier *self, const char *key) {
    struct Notifier *this = (struct Notifier *) self;
    this->multitonKey = strdup(key);
}

static void sendNotification(const struct INotifier *self, const char *notificationName, void *body, const char *type) {
    const struct IFacade *facade = self->getFacade(self);
    facade->sendNotification(facade, notificationName, body, type);
}

static struct Notifier *init(struct Notifier *notifier) {
    if (notifier == NULL) return NULL;
    notifier->base.getFacade = getFacade;
    notifier->base.getMultitonKey = getMultitonKey;
    notifier->base.initializeNotifier = initializeNotifier;
    notifier->base.sendNotification = sendNotification;
    return notifier;
}

static struct Notifier *alloc() {
    struct Notifier *notifier = malloc(sizeof(struct Notifier));
    if (notifier == NULL) {
        fprintf(stderr, "Notifier allocation failed.\n");
        return NULL;
    }
    memset(notifier, 0, sizeof(*notifier));

    return notifier;
}

struct INotifier *puremvc_notifier_new() {
    return (struct INotifier *) init(alloc());
}

void puremvc_notifier_free(struct INotifier **notifier) {
    if (notifier == NULL || *notifier == NULL) return;
    struct Notifier *this = (struct Notifier *) *notifier;

    free((void *) this->multitonKey);

    free(this);
    *notifier = NULL;
}
