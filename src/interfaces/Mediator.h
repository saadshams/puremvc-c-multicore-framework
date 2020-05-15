#ifndef PUREMVC_MEDIATOR_H
#define PUREMVC_MEDIATOR_H

#include "Notification.h"

struct Notifier;

typedef struct Mediator Mediator;

struct Mediator {
    struct Notifier *notifier;
    const char *mediatorName;
    void *viewComponent;
    const char *(*getMediatorName)(Mediator *self);
    void (*setViewComponent)(Mediator *self, void *viewComponent);
    void *(*getViewComponent)(Mediator *self);
    const char * const *(*listNotificationInterests)(Mediator *self);
    void (*handleNotification)(Mediator *self, Notification *notification);
    void (*onRegister)(Mediator *self);
    void (*onRemove)(Mediator *self);
};

#define MEDIATOR_NAME "Mediator"

void InitMediator(Mediator *self, const char *mediatorName, void *viewComponent);

Mediator *NewMediator(const char *mediatorName, void *viewComponent);

void DeleteMediator(Mediator *self);

#endif //PUREMVC_MEDIATOR_H
