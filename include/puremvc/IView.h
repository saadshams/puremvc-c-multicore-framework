#pragma once

#include <stdbool.h>

#include "IMediator.h"
#include "INotification.h"
#include "IObserver.h"

struct IView {
    void (*initializeView)(struct IView *self);

    void (*registerObserver)(const struct IView *self, const char *notificationName, const struct IObserver *observer);
    void (*notifyObservers)(const struct IView *self, const struct INotification *notification);
    void (*removeObserver)(const struct IView *self, const char *notificationName, const void *notifyContext);

    void (*registerMediator)(const struct IView *self, struct IMediator *mediator);
    struct IMediator *(*retrieveMediator)(const struct IView *self, const char *mediatorName);
    bool (*hasMediator)(const struct IView *self, const char *mediatorName);
    struct IMediator *(*removeMediator)(const struct IView *self, const char *mediatorName);
};

struct IView *puremvc_view_new(const char *key);
void puremvc_view_free(struct IView **view);

struct IView *puremvc_view_getInstance(const char *key, struct IView *(*factory)(const char *));
void puremvc_view_removeView(const char *key);
