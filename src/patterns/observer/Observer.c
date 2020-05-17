#include "interfaces/Observer.h"
#include <stdio.h>
#include <stdlib.h>

static void *getNotifyContext(Observer *self) {
    return self->context;
}

static void setNotifyContext(Observer *self, void *notifyContext) {
    self->context = notifyContext;
}

static void (*getNotifyMethod(Observer *self))(void *context, Notification *notification) {
    return self->notify;
}

static void setNotifyMethod(Observer *self, void (*notifyMethod)(void *context, Notification *notification)) {
    self->notify = notifyMethod;
}

static void notifyObserver(Observer *self, Notification *notification) {
    self->notify(self->context, notification);
}

static bool compareNotifyContext(Observer *self, const void *context) {
    return self->context == context;
}

Observer *NewObserver(void (*notifyMethod)(void *context, Notification *notification), void *notifyContext) {
    Observer *observer = malloc(sizeof(Observer));
    if (observer == NULL) goto exception;
    InitObserver(observer, notifyMethod, notifyContext);
    return observer;

    exception:
        fprintf(stderr, "Observer allocation failed.\n");
        return NULL;
}

void InitObserver(Observer *observer, void (*notifyMethod)(void *context, Notification *notification), void *notifyContext) {
    observer->notify = notifyMethod;
    observer->context = notifyContext;
    observer->getNotifyContext = getNotifyContext;
    observer->setNotifyContext = setNotifyContext;
    observer->getNotifyMethod = getNotifyMethod;
    observer->setNotifyMethod = setNotifyMethod;
    observer->notifyObserver = notifyObserver;
    observer->compareNotifyContext = compareNotifyContext;
}

void DeleteObserver(Observer *observer) {
    free(observer);
    observer = NULL;
}
