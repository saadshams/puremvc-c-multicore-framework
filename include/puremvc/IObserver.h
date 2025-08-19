#pragma once

#include <stdbool.h>

#include "INotification.h"

struct IObserver {
    void *(*getContext)(const struct IObserver *observer);
    void (*setContext)(struct IObserver *observer, void *notifyContext);

    void (*(*getNotify)(const struct IObserver *self))(const void *context, struct INotification *notification);
    void (*setNotify)(struct IObserver *observer, void (*notify)(const void *context, struct INotification *notification));

    void (*notifyObserver)(const struct IObserver *observer, struct INotification *notification);
    bool (*compareNotifyContext)(const struct IObserver *observer, const void *context);
};

struct IObserver *puremvc_observer_new(const void (*notify)(const void *context, struct INotification *notification), const void *context);
void puremvc_observer_free(struct IObserver **observer);
