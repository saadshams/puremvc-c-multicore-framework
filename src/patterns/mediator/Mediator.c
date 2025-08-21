#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "Mediator.h"

static const char *getName(const struct IMediator *self) {
    const struct Mediator *this = (struct Mediator *) self;
    return this->name;
}

static void setComponent(struct IMediator *self, void *component) {
    struct Mediator *this = (struct Mediator *) self;
    this->component = component;
}

static void *getComponent(const struct IMediator *self) {
    const struct Mediator *this = (struct Mediator *) self;
    return this->component;
}

static char **allocNotificationInterests(const struct IMediator *self, const char **notifications) {
    size_t count = 0;
    while (notifications[count] != NULL) count++;

    char **interests = malloc(sizeof(char*) * (count + 1));
    if (interests == NULL) {
        fprintf(stderr, "Notification interest allocation failed.\n");
        return NULL;
    }
    interests[count] = NULL;

    for (size_t i = 0; i < count; i++) {
        interests[i] = strdup(notifications[i]);
        if (interests[i] == NULL) {
            fprintf(stderr, "Notification string duplication failed.\n");
            for (size_t j = 0; j < i; j++) free(interests[j]); // Free previously allocated strings
            free(interests);
            return NULL;
        }
    }
    return interests;
}

static void freeNotificationInterests(const struct IMediator *self, char **interests) {
    for (char **cursor = interests; *cursor; cursor++) free(*cursor);
    free(interests);
}

static char **listNotificationInterests(const struct IMediator *self) {
    static const char *interests[] = { NULL };
    return self->allocNotificationInterests(self, interests);
}

static void handleNotification(const struct IMediator *self, struct INotification *notification) {

}

static void onRegister(struct IMediator *self) {

}

static void onRemove(struct IMediator *self) {

}

static struct Mediator *init(struct Mediator *mediator) {
    if (mediator == NULL) return NULL;
    mediator->base.getName = getName;
    mediator->base.setComponent = setComponent;
    mediator->base.getComponent = getComponent;
    mediator->base.allocNotificationInterests = allocNotificationInterests;
    mediator->base.freeNotificationInterests = freeNotificationInterests;
    mediator->base.listNotificationInterests = listNotificationInterests;
    mediator->base.handleNotification = handleNotification;
    mediator->base.onRegister = onRegister;
    mediator->base.onRemove = onRemove;
    return mediator;
}

static struct Mediator *alloc(const char *name, void *component) {
    struct Mediator *mediator = malloc(sizeof(struct Mediator));
    if (mediator == NULL) {
        fprintf(stderr, "Mediator allocation failed.\n");
        return NULL;
    };

    memset(mediator, 0, sizeof(struct Mediator));
    mediator->base.notifier = puremvc_notifier_new();
    mediator->name = strdup(name ? name : MEDIATOR_NAME);
    if (mediator->name == NULL) {
        fprintf(stderr, "Proxy allocation failed: strdup failed\n");
        free(mediator);
        return NULL;
    }
    mediator->component = component;
    return mediator;
}

struct IMediator *puremvc_mediator_new(const char *name, void *component) {
    return (struct IMediator *) init(alloc(name, component));
}

void puremvc_mediator_free(struct IMediator **mediator) {
    if (mediator == NULL || *mediator == NULL) return;

    struct Mediator *this = (struct Mediator *) *mediator;
    free((void *) this->name);
    puremvc_notifier_free(&this->base.notifier);
    free(this);

    *mediator = NULL;
}
