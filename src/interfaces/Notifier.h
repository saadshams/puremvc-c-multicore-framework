#ifndef PUREMVC_NOTIFIER_H
#define PUREMVC_NOTIFIER_H

#include "Facade.h"

typedef struct Notifier Notifier;

struct Notifier {
    const char *multitonKey;
    Facade *(*getFacade)(Notifier *self);
    void (*initializeNotifier)(Notifier *self, const char *key);
    void (*sendNotification)(Notifier *self, const char *notificationName, void *body, char *type);
};

void InitNotifier(Notifier *self);

Notifier *NewNotifier();

void DeleteNotifier(Notifier *self);

#endif //PUREMVC_NOTIFIER_H
