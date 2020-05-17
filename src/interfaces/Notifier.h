#ifndef PUREMVC_NOTIFIER_H
#define PUREMVC_NOTIFIER_H

#include "Facade.h"

/**
 * <P>A Base <code>INotifier</code> implementation.</P>
 *
 * <P><code>MacroCommand, Command, Mediator</code> and <code>Proxy</code>
 * all have a need to send <code>Notifications</code>.</P>
 *
 * <P>The <code>INotifier</code> interface provides a common method called
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
 * @see org.puremvc.c.multicore.patterns.proxy.Proxy Proxy
 * @see org.puremvc.c.multicore.patterns.facade.Facade Facade
 * @see org.puremvc.c.multicore.patterns.mediator.Mediator Mediator
 * @see org.puremvc.c.multicore.patterns.command.MacroCommand MacroCommand
 * @see org.puremvc.c.multicore.patterns.command.SimpleCommand SimpleCommand
 */
typedef struct Notifier Notifier;

struct Notifier {
    const char *multitonKey;
    Facade *(*getFacade)(Notifier *self);
    void (*initializeNotifier)(Notifier *self, const char *key);
    void (*sendNotification)(Notifier *self, const char *notificationName, void *body, char *type);
};

/**
 * Constructor
 */
Notifier *NewNotifier();

/**
 * Initializer
 *
 * @param notifier
 */
void InitNotifier(Notifier *notifier);

/**
 * Destructor
 *
 * @param notifier
 */
void DeleteNotifier(Notifier *notifier);

#endif //PUREMVC_NOTIFIER_H
