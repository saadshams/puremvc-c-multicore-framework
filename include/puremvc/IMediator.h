#pragma once

#include "INotification.h"
#include "INotifier.h"

#define MEDIATOR_NAME "mediator"

struct IMediator {
    struct INotifier *notifier;

    const char *(*getName)(const struct IMediator *self);
    void (*setComponent)(struct IMediator *self, void *component);
    void *(*getComponent)(const struct IMediator *self);

    char **(*allocNotificationInterests)(const struct IMediator *self, const char **interests);
    void (*freeNotificationInterests)(const struct IMediator *self, char **interests);

    char **(*listNotificationInterests)(const struct IMediator *self);
    void (*handleNotification)(const struct IMediator *self, struct INotification *notification);

    void (*onRegister)(struct IMediator *self);
    void (*onRemove)(struct IMediator *self);
};

struct IMediator *puremvc_mediator_new(const char *name, void *component);
void puremvc_mediator_free(struct IMediator **mediator);
