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

void InitObserver(Observer *self, void (*notifyMethod)(void *context, Notification *notification), void *notifyContext) {
    self->notify = notifyMethod;
    self->context = notifyContext;
    self->getNotifyContext = getNotifyContext;
    self->setNotifyContext = setNotifyContext;
    self->getNotifyMethod = getNotifyMethod;
    self->setNotifyMethod = setNotifyMethod;
    self->notifyObserver = notifyObserver;
    self->compareNotifyContext = compareNotifyContext;
}

Observer *NewObserver(void (*notifyMethod)(void *context, Notification *notification), void *notifyContext) {
    Observer *self = malloc(sizeof(Observer));
    if (self == NULL) goto exception;
    InitObserver(self, notifyMethod, notifyContext);
    return self;

    exception:
        fprintf(stderr, "Observer allocation failed.\n");
        return NULL;
}

void DeleteObserver(Observer *self) {
    free(self);
    self = NULL;
}
