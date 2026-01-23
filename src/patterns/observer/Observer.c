/**
* @file Observer.c
* @internal
* @brief Observer Implementation
*
* @author Saad Shams <saad.shams@puremvc.org>
* @copyright BSD 3-Clause License
*/
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

static void (*getNotify(const struct IObserver *self))(const void *context, struct INotification *notification, const char **error) {
    const struct Observer *this = (struct Observer *) self;
    return this->notify;
}

static void setNotify(struct IObserver *self, void (*notify)(const void *context, struct INotification *notification, const char **error)) {
    struct Observer *this = (struct Observer *) self;
    this->notify = notify;
}

static void notifyObserver(const struct IObserver *self, struct INotification *notification, const char **error) {
    const struct Observer *this = (struct Observer *) self;
    if (this->notify == NULL && this->context == NULL)
        return *error = "[PureMVC::Observer::notifyObserver] Error: Notify and Context must not be NULL", (void)0;

    this->notify(this->context, notification, error);
}

static bool compareNotifyContext(const struct IObserver *self, const void *context) {
    const struct Observer *this = (struct Observer *) self;
    if (this->context == NULL || context == NULL) return false;

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

static struct Observer *alloc(const void (*notify)(const void *context, struct INotification *notification), const void *context, const char **error) {
    struct Observer *observer = malloc(sizeof(struct Observer));
    if (observer == NULL) return *error = "[PureMVC::Observer::alloc] Error: Failed to allocate Observer.", NULL;

    memset(observer, 0, sizeof(struct Observer));

    observer->notify = (void *)notify;
    observer->context = (void *)context;
    return observer;
}

struct IObserver *puremvc_observer_new(const void (*notify)(const void *context, struct INotification *notification), const void *context, const char **error) {
    return (struct IObserver *) init(alloc(notify, context, error));
}

void puremvc_observer_free(struct IObserver **observer) {
    if (observer == NULL || *observer == NULL) return;
    struct Observer *this = (struct Observer *) *observer;

    free(this);
    *observer = NULL;
}
