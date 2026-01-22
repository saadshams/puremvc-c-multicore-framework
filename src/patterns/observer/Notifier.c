/**
* @file Notifier.c
* @internal
* @brief Notifier Implementation
*
* @author Saad Shams <saad.shams@puremvc.org>
* @copyright BSD 3-Clause License
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "Notifier.h"
#include "puremvc/IFacade.h"

static struct IFacade *getFacade(const struct INotifier *self) {
    const struct Notifier *this = (struct Notifier *) self;
    return puremvc_facade_getInstance(this->key, puremvc_facade_new);
}

const char *getMultitonKey(const struct INotifier *self) {
    const struct Notifier *this = (struct Notifier *) self;
    return this->key;
}

static void initializeNotifier(struct INotifier *self, const char *key) {
    if (key == NULL) {
        fprintf(stderr, "[PureMVC::Notifier::initializeNotifier] Error: key must not be NULL.\n");
        exit(EXIT_FAILURE);
    }

    struct Notifier *this = (struct Notifier *) self;

    this->key = strdup(key);
    if (this->key == NULL) {
        fprintf(stderr, "[PureMVC::Notifier::%s] Error: strdup failed for key '%s'.\n", __func__, key);
    }
}

static void sendNotification(const struct INotifier *self, const char *notificationName, void *body, const char *type) {
    const struct IFacade *facade = self->getFacade(self);
    facade->sendNotification(facade, notificationName, body, type);
}

static struct Notifier *init(struct Notifier *notifier) {
    if (notifier == NULL) return NULL;
    notifier->base.getFacade = getFacade;
    notifier->base.getKey = getMultitonKey;
    notifier->base.initializeNotifier = initializeNotifier;
    notifier->base.sendNotification = sendNotification;
    return notifier;
}

static struct Notifier *alloc(const char **error) {
    struct Notifier *notifier = malloc(sizeof(struct Notifier));
    if (notifier == NULL) return *error = "[PureMVC::Notifier::alloc] Error: Failed to allocate Notifier.", NULL;

    memset(notifier, 0, sizeof(*notifier));

    return notifier;
}

struct INotifier *puremvc_notifier_new(const char **error) {
    return (struct INotifier *) init(alloc(error));
}

void puremvc_notifier_free(struct INotifier **notifier) {
    if (notifier == NULL || *notifier == NULL) return;
    struct Notifier *this = (struct Notifier *) *notifier;

    free((void *) this->key);

    free(this);
    *notifier = NULL;
}
