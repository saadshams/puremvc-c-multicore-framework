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
 * @param observer
 * @param notifyMethod
 * @param notifyContext
 */
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
    Observer *observer = malloc(sizeof(Observer));
    if (observer == NULL) goto exception;
    InitObserver(observer, notifyMethod, notifyContext);
    return observer;

    exception:
        fprintf(stderr, "Observer allocation failed.\n");
        return NULL;
}

/**
 * Destructor
 * @param observer
 */
void DeleteObserver(Observer *observer) {
    free(observer);
    observer = NULL;
}
