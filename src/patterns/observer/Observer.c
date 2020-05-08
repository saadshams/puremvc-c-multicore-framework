#include "interfaces/Observer.h"
#include <stdlib.h>

static void *getNotifyContext(const Observer *self) {
    return self->context;
}

static void setNotifyContext(Observer *self, void *notifyContext) {
    self->context = notifyContext;
}

static void (*getNotifyMethod(const Observer *self))(void *context, Notification *notification) {
    return self->notify;
}

static void setNotifyMethod(Observer *self, void (*notifyMethod)(void *context, Notification *notification)) {
    self->notify = notifyMethod;
}

static void notifyObserver(const Observer *self, Notification *notification) {
    self->notify(self->context, notification);
}

static bool compareNotifyContext(const Observer *self, const void *context) {
    return self->context == context;
}

void InitObserver(Observer *self, void (*notifyMethod)(void *context, Notification *notification), void *notifyContext) {
    if (self) {
        self->notify = notifyMethod;
        self->context = notifyContext;
        self->getNotifyContext = getNotifyContext;
        self->setNotifyContext = setNotifyContext;
        self->getNotifyMethod = getNotifyMethod;
        self->setNotifyMethod = setNotifyMethod;
        self->notifyObserver = notifyObserver;
        self->compareNotifyContext = compareNotifyContext;
    }
}

Observer *NewObserver(void (*notifyMethod)(void *context, Notification *notification), void *notifyContext) {
    Observer *self = malloc(sizeof(Observer));
    InitObserver(self, notifyMethod, notifyContext);
    return self;
}

void DeleteObserver(Observer *self) {
    free(self);
}
