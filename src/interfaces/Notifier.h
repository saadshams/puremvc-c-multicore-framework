#ifndef PUREMVC_NOTIFIER_H
#define PUREMVC_NOTIFIER_H

#include "Facade.h"

/**
 * <P>A Base <code>Notifier</code> implementation.</P>
 *
 * <P><code>MacroCommand, Command, Mediator</code> and <code>Proxy</code>
 * all have a need to send <code>Notifications</code>.</P>
 *
 * <P>The <code>Notifier</code> interface provides a common method called
 * <code>sendNotification</code> that relieves implementation code of
 * the necessity to actually construct <code>Notifications</code>.</P>
 *
 * <P>The <code>Notifier</code> class, which all of the above mentioned classes
 * extend, provides an initialized reference to the <code>Facade</code>
 * Multiton, which is required for the convenience method
 * for sending <code>Notifications</code>, but also eases implementation as these
 * classes have frequent <code>Facade</code> interactions and usually require
 * access to the facade anyway.</P>
 *
 * <P>NOTE: In the MultiCore version of the framework, there is one caveat to
 * notifiers, they cannot send notifications or reach the facade until they
 * have a valid multitonKey.</P>
 *
 * <P>The multitonKey is set:
 *   * on a Command when it is executed by the Controller
 *   * on a Mediator is registered with the View
 *   * on a Proxy is registered with the Model.</P>
 *
 * @see Proxy
 * @see Facade
 * @see Mediator
 * @see MacroCommand
 * @see SimpleCommand
 */
typedef struct Notifier Notifier;

struct Notifier {

    // The Multiton Key for this app
    const char *multitonKey;

    /** Return the Multiton Facade instance */
    Facade *(*getFacade)(Notifier *self);

    /**
     * Initialize this INotifier instance.
     * <P>
     * This is how a Notifier gets its multitonKey.
     * Calls to sendNotification or to access the
     * facade will fail until after this method
     * has been called.</P>
     *
     * <P>
     * Mediators, Commands or Proxies may override
     * this method in order to send notifications
     * or access the Multiton Facade instance as
     * soon as possible. They CANNOT access the facade
     * in their constructors, since this method will not
     * yet have been called.</P>
     *
     * @param key the multitonKey for this INotifier to use
     */
    void (*initializeNotifier)(Notifier *self, const char *key);

    /**
     * Create and send an <code>INotification</code>.
     *
     * <P>
     * Keeps us from having to construct new INotification
     * instances in our implementation code.
     *
     * @param self Notifier
     * @param notificationName the name of the notification to send
     * @param body the body of the notification (optional)
     * @param type the type of the notification (optional)
     */
    void (*sendNotification)(Notifier *self, const char *notificationName, void *body, char *type);
};

struct $Notifier {
    /** Constructor */
    Notifier *(*new)(void);

    /**
     * Initializer
     *
     * @param notifier
     */
    void (*init)(Notifier *notifier);

    /**
     * Destructor
     *
     * @param notifier
     */
    void (*delete)(Notifier *notifier);
};

const struct $Notifier $Notifier;

#endif //PUREMVC_NOTIFIER_H
