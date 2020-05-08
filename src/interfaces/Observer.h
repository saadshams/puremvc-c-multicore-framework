#ifndef PUREMVC_OBSERVER_H
#define PUREMVC_OBSERVER_H

#include "Notification.h"
#include <stdbool.h>

typedef struct Observer Observer;

struct Observer {
    void *context;
    void (*notify)(void *context, Notification *notification);

    void *(*getNotifyContext)(const Observer *self);
    void (*setNotifyContext)(Observer *self, void *notifyContext);

    void (*(*getNotifyMethod)(const Observer *self))(void *context, Notification *notification);
    void (*setNotifyMethod)(Observer *self, void (*notifyMethod)(void *context, Notification *notification));

    void (*notifyObserver)(const Observer *self, Notification *notification);
    bool (*compareNotifyContext)(const Observer *self, const void *context);
};

void InitObserver(Observer *self, void (*notifyMethod)(void *context, Notification *notification), void *notifyContext);

Observer *NewObserver(void (*notifyMethod)(void *context, Notification *notification), void *notifyContext);

void DeleteObserver(Observer *self);

#endif //PUREMVC_OBSERVER_H
