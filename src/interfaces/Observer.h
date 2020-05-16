#ifndef PUREMVC_OBSERVER_H
#define PUREMVC_OBSERVER_H

#include "Notification.h"
#include <stdbool.h>

/**
 * <P>A base <code>IObserver</code> implementation.</P>
 *
 * <P>An <code>Observer</code> is an object that encapsulates information
 * about an interested object with a method that should
 * be called when a particular <code>INotification</code> is broadcast.</P>
 *
 * <P>In PureMVC, the <code>Observer</code> class assumes these responsibilities:</P>
 *
 * <UL>
 * <LI>Encapsulate the notification (callback) method of the interested object.</LI>
 * <LI>Encapsulate the notification context (this) of the interested object.</LI>
 * <LI>Provide methods for setting the notification method and context.</LI>
 * <LI>Provide a method for notifying the interested object.</LI>
 * </UL>
 *
 * @see org.puremvc.c.multicore.core.View View
 * @see org.puremvc.c.multicore.patterns.observer.Notification Notification
 */
typedef struct Observer Observer;

struct Observer {
    void *context;
    void (*notify)(void *context, Notification *notification);
    void *(*getNotifyContext)(Observer *self);
    void (*setNotifyContext)(Observer *self, void *notifyContext);
    void (*(*getNotifyMethod)(Observer *self))(void *context, Notification *notification);
    void (*setNotifyMethod)(Observer *self, void (*notifyMethod)(void *context, Notification *notification));
    void (*notifyObserver)(Observer *self, Notification *notification);
    bool (*compareNotifyContext)(Observer *self, const void *context);
};

/**
 * Initializer
 *
 * @param self
 * @param notifyMethod
 * @param notifyContext
 */
void InitObserver(Observer *self, void (*notifyMethod)(void *context, Notification *notification), void *notifyContext);

/**
 * Constructor
 *
 * @param notifyMethod
 * @param notifyContext
 */
Observer *NewObserver(void (*notifyMethod)(void *context, Notification *notification), void *notifyContext);

/**
 * Destructor
 *
 * @param self
 */
void DeleteObserver(Observer *self);

#endif //PUREMVC_OBSERVER_H
