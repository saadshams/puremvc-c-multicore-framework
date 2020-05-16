#include "interfaces/Notifier.h"
#include <stdio.h>
#include <string.h>

/**
 *
 * @param self
 * @return Facade
 */
Facade *getFacade(Notifier *self) {
    return getFacadeInstance(self->multitonKey, NewFacade);
}

/**
 * <P>Create and send an <code>INotification</code>.</P>
 *
 * <P>Keeps us from having to construct new INotification
 * instances in our implementation code.</P>
 *
 * @param self
 * @param notificationName the name of the notification to send
 * @param body the body of the notification
 * @param type the type of the notification
 */
static void sendNotification(Notifier *self, const char *notificationName, void *body, char *type) {
    Facade *facade = self->getFacade(self);
    facade->sendNotification(facade, notificationName, body, type);
}

/**
 * <P>Initialize this INotifier instance.</P>
 *
 * <P>This is how a Notifier gets its multitonKey.
 * Calls to sendNotification or to access the
 * facade will fail until after this method
 * has been called.</P>
 *
 * <P>Mediators, Commands or Proxies may override
 * this method in order to send notifications
 * or access the Multiton Facade instance as
 * soon as possible. They CANNOT access the facade
 * in their constructors, since this method will not
 * yet have been called.</P>
 *
 * @param self
 * @param key the multitonKey for this INotifier to use
 */
static void initializeNotifier(Notifier *self, const char *key) {
    self->multitonKey = key;
}

/**
 * Initializer
 *
 * @param self
 */
void InitNotifier(Notifier *self) {
    self->multitonKey = NULL;
    self->getFacade = getFacade;
    self->initializeNotifier  = initializeNotifier;
    self->sendNotification = sendNotification;
}

/**
 * Constructor
 */
Notifier *NewNotifier() {
    Notifier *self = malloc(sizeof(Notifier));
    if (self == NULL) goto exception;
    InitNotifier(self);
    return self;

    exception:
        fprintf(stderr, "Notifier allocation failed.\n");
        return NULL;
}

/**
 * Destructor
 *
 * @param self
 */
void DeleteNotifier(Notifier *self) {
    free(self);
    self = NULL;
}
