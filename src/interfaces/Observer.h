#ifndef PUREMVC_OBSERVER_H
#define PUREMVC_OBSERVER_H

#include "Notification.h"
#include <stdbool.h>

/**
 * <P>A base <code>Observer</code> implementation.</P>
 *
 * <P>An <code>Observer</code> is an object that encapsulates information
 * about an interested object with a method that should
 * be called when a particular <code>Notification</code> is broadcast.</P>
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
 * @see View
 * @see Notification
 */
typedef struct Observer Observer;

struct Observer {

    void *context;

    void (*notify)(void *context, Notification *notification);

    /**
     * Get the notification context.
     *
     * @param self Observer
     * @return context
     */
    void *(*getNotifyContext)(Observer *self);

    /**
     * Set the notification context.
     *
     * @param self Observer
     * @param notifyContext the notification context (this) of the interested object.
     */
    void (*setNotifyContext)(Observer *self, void *notifyContext);

    /**
     * Get the notification method.
     *
     * @return the notification (callback) method of the interested object
     */
    void (*(*getNotifyMethod)(Observer *self))(void *context, Notification *notification);

    /**
     * Set the notification method.
     *
     * <P>The notification method should take one parameter of type <code>Notification</code>.</P>
     *
     * @param self Observer
     * @param notifyMethod
     */
    void (*setNotifyMethod)(Observer *self, void (*notifyMethod)(void *context, Notification *notification));

    /**
     * Notify the interested object.
     *
     * @param self Observer
     * @param notification Notification
     */
    void (*notifyObserver)(Observer *self, Notification *notification);

    /**
     * Compare an object to the notification context.
     *
     * @param self Observer
     * @param object the object to compare
     * @return boolean indicating if the object and the notification context are the same
     */
    bool (*compareNotifyContext)(Observer *self, const void *context);
};

struct $Observer {
    /**
     * Constructor.
     *
     * <P> The notification method on the interested object should take
     * one parameter of type <code>INotification</code></P>
     *
     * @param notifyMethod the notification method of the interested object
     * @param notifyContext the notification context of the interested object
     */
    Observer *(*new)(void (*notifyMethod)(void *context, Notification *notification), void *notifyContext);

    /** Initializer */
    void
    (*init)(Observer *observer, void (*notifyMethod)(void *context, Notification *notification), void *notifyContext);

    /** Destructor */
    void (*delete)(Observer *observer);
};

const struct $Observer $Observer;

#endif //PUREMVC_OBSERVER_H
