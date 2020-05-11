#ifndef PUREMVC_MEDIATOR_H
#define PUREMVC_MEDIATOR_H

#include "Notification.h"

struct Notifier;

typedef struct Mediator Mediator;

struct Mediator {
    struct Notifier *notifier;
    char *mediatorName;
    void *viewComponent;
    char *(*getMediatorName)(const Mediator *self);
    void (*setViewComponent)(Mediator *self, void *viewComponent);
    void *(*getViewComponent)(const Mediator *self);
    const char * const *(*listNotificationInterests)(const Mediator *self);
    void (*handleNotification)(const Mediator *self, Notification *notification);
    void (*onRegister)(Mediator *self);
    void (*onRemove)(Mediator *self);
};

static char *MEDIATOR_NAME = "Mediator";

void InitMediator(Mediator *self, char *mediatorName, void *viewComponent);

Mediator *NewMediator(char *mediatorName, void *viewComponent);

void DeleteMediator(Mediator *self);

#endif //PUREMVC_MEDIATOR_H
