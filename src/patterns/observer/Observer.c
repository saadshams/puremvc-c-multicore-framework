#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Observer.h"

static void *getContext(const struct IObserver *self) {
    const struct Observer *this = (struct Observer *) self;
    return this->context;
}

static void setContext(struct IObserver *self, void *notifyContext) {
    struct Observer *this = (struct Observer *) self;
    this->context = notifyContext;
}

static void (*getNotify(const struct IObserver *self))(const void *context, struct INotification *notification) {
    const struct Observer *this = (struct Observer *) self;
    return this->notify;
}

static void setNotify(struct IObserver *self, void (*notify)(const void *context, struct INotification *notification)) {
    struct Observer *this = (struct Observer *) self;
    this->notify = notify;
}

static void notifyObserver(const struct IObserver *self, struct INotification *notification) {
    const struct Observer *this = (struct Observer *) self;
    this->notify(this->context, notification);
}

static bool compareNotifyContext(const struct IObserver *self, const void *context) {
    const struct Observer *this = (struct Observer *) self;
    return this->context == context;
}

static struct Observer *init(struct Observer *observer) {
    if (observer == NULL) return NULL;
    observer->base.getContext = getContext;
    observer->base.setContext = setContext;
    observer->base.getNotify = getNotify;
    observer->base.setNotify = setNotify;
    observer->base.notifyObserver = notifyObserver;
    observer->base.compareNotifyContext = compareNotifyContext;
    return observer;
}

static struct Observer *alloc(const void (*notify)(const void *context, struct INotification *notification), const void *context) {
    struct Observer *observer = malloc(sizeof(struct Observer));
    if (observer == NULL) {
        fprintf(stderr, "Observer allocation failed.\n");
        return NULL;
    }

    memset(observer, 0, sizeof(struct Observer));

    observer->notify = (void *)notify;
    observer->context = (void *)context;
    return observer;
}

struct IObserver *puremvc_observer_new(const void (*notify)(const void *context, struct INotification *notification), const void *context) {
    return (struct IObserver *) init(alloc(notify, context));
}

void puremvc_observer_free(struct IObserver **observer) {
    if (observer == NULL || *observer == NULL) return;

    struct Observer *this = (struct Observer *) *observer;
    free(this);

    *observer = NULL;
}
