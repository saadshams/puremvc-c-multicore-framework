#ifndef PUREMVC_CONTROLLER_H
#define PUREMVC_CONTROLLER_H

#include "Map.h"
#include "SimpleCommand.h"
#include "View.h"
#include <stdbool.h>

typedef struct Controller Controller;

/**
 * <P>A Multiton <code>Controller</code> implementation.</P>
 *
 * <P>In PureMVC, the <code>Controller</code> class follows the
 * 'Command and Controller' strategy, and assumes these
 * responsibilities:</P>
 *
 * <UL>
 * <LI> Remembering which <code>Command</code>
 * are intended to handle which <code>Notifications</code>.</LI>
 * <LI> Registering itself as an <code>Observer</code> with
 * the <code>View</code> for each <code>Notification</code>
 * that it has an <code>Command</code> mapping for.</LI>
 * <LI> Creating a new instance of the proper <code>Command</code>
 * to handle a given <code>Notification</code> when notified by the <code>View</code>.</LI>
 * <LI> Calling the <code>Command</code>'s <code>execute</code>
 * method, passing in the <code>Notification</code>.</LI>
 * </UL>
 *
 * <P>Your application must register <code>Commands</code> with the
 * Controller.</P>
 *
 * <P>The simplest way is to subclass <code>Facade</code>,
 * and use its <code>initializeController</code> method to add your
 * registrations.</P>
 *
 * @see View View
 * @see Observer
 * @see Notification
 * @see SimpleCommand
 * @see MacroCommand
 */
struct Controller {

    const char *multitonKey;

    View *view;

    Map *commandMap;

    /**
     * <P>Initialize the Multiton <code>Controller</code> instance.</P>
     *
     * <P>Called automatically by the constructor.</P>
     *
     * <P>Note that if you are using a subclass of <code>View</code>
     * in your application, you should <i>also</i> subclass <code>Controller</code>
     * and override the <code>initializeController</code> method in the
     * following way:</P>
     *
     * ```c
     * // ensure that the Controller is talking to my View implementation
     * static void initializeController()
     * {
     *     view = getViewInstance(self->multitonKey, NewMyView);
     * }
     * ```
     *
     * @param self Controller
     */
    void (*initializeController)(Controller *self);

    /**
     * <P>If an <code>Command</code> has previously been registered
     * to handle a the given <code>Notification</code>, then it is executed.</P>
     *
     * @param self Controller
     * @param notification an <code>Notification</code>
     */
    void (*executeCommand)(Controller *self, Notification *notification);

    /**
     * <P>Register a particular <code>Command</code> class as the handler
     * for a particular <code>Notification</code>.</P>
     *
     * <P>If an <code>Command</code> has already been registered to
     * handle <code>Notification</code>s with this name, it is no longer
     * used, the new <code>Command</code> is used instead.</P>
     *
     * <P>The Observer for the new Command is only created if this the
     * first time an Command has been registered for this Notification name.</P>
     *
     * @param self Controller
     * @param notificationName the name of the <code>Notification</code>
     * @param factory a reference to <code>Command</code> factory
     */
    void (*registerCommand)(Controller *self, const char *notificationName, SimpleCommand *(*factory)(void));

    /**
     * <P>Check if a Command is registered for a given Notification</P>
     *
     * @param self Controller
     * @param notificationName notification name
     * @return whether a Command is currently registered for the given <code>notificationName</code>.
     */
    bool (*hasCommand)(Controller *self, const char *notificationName);

    /**
     * <P>Remove a previously registered <code>Command</code> to <code>Notification</code> mapping.</P>
     *
     * @param self Controller
     * @param notificationName the name of the <code>Notification</code> to remove the <code>Command</code> mapping for
     */
    void (*removeCommand)(Controller *self, const char *notificationName);
};

struct $Controller {
    /**
     * <P><code>Controller</code> Multiton Factory method.</P>
     *
     * @param key multitonKey
     * @param factory factory that returns <code>Controller</code>
     * @return the Multiton instance of <code>Controller</code>
     */
    Controller *(*getInstance)(const char *key, Controller *(*factory)(const char *));

    /**
     * Constructor
     *
     * <P>This <code>Controller</code> implementation is a Multiton,
     * so you should not call the constructor
     * directly, but instead call the static Factory method,
     * passing the unique key and a supplier for this instance
     * <code>getControllerInstance(multitonKey, NewController)</code></P>
     *
     * @param key
     * @return instance of <code>Controller</code>
     * @throws Error if instance for this Multiton key has already been constructed
     */
    Controller *(*new)(const char *key);

    /** Initializer */
    void (*init)(Controller *controller);

    /** Remove Core */
    void (*removeController)(const char *key);
};

const struct $Controller $Controller;

#endif //PUREMVC_CONTROLLER_H
