#include "interfaces/Observer.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * Get the notification context.
 *
 * @param self
 * @return notification context
 */
static void *getNotifyContext(Observer *self) {
    return self->context;
}

/**
 * Set the notification context.
 *
 * @param self
 * @param notifyContext the notification context (this) of the interested object.
 */
static void setNotifyContext(Observer *self, void *notifyContext) {
    self->context = notifyContext;
}

/**
 * Get the notification method.
 *
 * @return the notification (callback) method of the interested object
 */
static void (*getNotifyMethod(Observer *self))(void *context, Notification *notification) {
    return self->notify;
}

/**
 * Set the notification method.
 *
 * <P>The notification method should take one parameter of type <code>INotification</code>.</P>
 *
 * @param self
 * @param notifyMethod
 */
static void setNotifyMethod(Observer *self, void (*notifyMethod)(void *context, Notification *notification)) {
    self->notify = notifyMethod;
}

/**
 * Notify the interested object.
 *
 * @param self
 * @param notification
 */
static void notifyObserver(Observer *self, Notification *notification) {
    self->notify(self->context, notification);
}

/**
 * Compare an object to the notification context.
 *
 * @param self
 * @param object the object to compare
 * @return boolean indicating if the object and the notification context are the same
 */

static bool compareNotifyContext(Observer *self, const void *context) {
    return self->context == context;
}

/**
 * Initializer
 * @param self
 * @param notifyMethod
 * @param notifyContext
 */
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

/**
 * Constructor.
 *
 * <P>
 * The notification method on the interested object should take
 * one parameter of type <code>INotification</code></P>
 *
 * @param notifyMethod the notification method of the interested object
 * @param notifyContext the notification context of the interested object
 */
Observer *NewObserver(void (*notifyMethod)(void *context, Notification *notification), void *notifyContext) {
    Observer *self = malloc(sizeof(Observer));
    if (self == NULL) goto exception;
    InitObserver(self, notifyMethod, notifyContext);
    return self;

    exception:
        fprintf(stderr, "Observer allocation failed.\n");
        return NULL;
}

/**
 * Destructor
 * @param self
 */
void DeleteObserver(Observer *self) {
    free(self);
    self = NULL;
}
