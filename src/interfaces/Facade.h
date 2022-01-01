#ifndef PUREMVC_FACADE_H
#define PUREMVC_FACADE_H

#include "Controller.h"
#include "Model.h"
#include "View.h"

/**
 * A base Multiton <code>Facade</code> implementation.
 *
 * @see Model
 * @see View
 * @see Controller
 */
typedef struct Facade Facade;

struct Facade {

    /** The Multiton Key for this app */
    const char *multitonKey;

    /** References to Model, View and Controller */
    Controller *controller;

    Model *model;

    View *view;

    /**
     * <P>Initialize the Multiton <code>Facade</code> instance.</P>
     *
     * <P>Called automatically by the constructor. Override in your
     * subclass to do any subclass specific initializations. Be
     * sure to call <code>initializeFacade(self)</code>, though.</P>
     *
     * @param self Facade
     */
    void (*initializeFacade)(Facade *self);

    /**
     * <P>Initialize the <code>Controller</code>.</P>
     *
     * <P>Called by the <code>initializeFacade</code> method.
     * Override this method in your subclass of <code>Facade</code>
     * if one or both of the following are true:</P>
     *
     * <UL>
     * <LI> You wish to initialize a different <code>Controller</code>.</LI>
     * <LI> You have <code>Commands</code> to register with the <code>Controller</code> at startup.. </LI>
     * </UL>
     *
     * <P>If you don't want to initialize a different <code>Controller</code>,
     * call <code>initializeController(self)</code> at the beginning of your
     * method, then register <code>Command</code>s.</P>
     *
     * @param self Facade
     */
    void (*initializeController)(Facade *self);

    /**
     * <P>Initialize the <code>Model</code>.</P>
     *
     * <P>Called by the <code>initializeFacade</code> method.
     * Override this method in your subclass of <code>Facade</code>
     * if one or both of the following are true:</P>
     *
     * <UL>
     * <LI> You wish to initialize a different <code>Model</code>.</LI>
     * <LI> You have <code>Proxy</code>s to register with the Model that do not
     * retrieve a reference to the Facade at construction time.</LI>
     * </UL>
     *
     * <P>If you don't want to initialize a different <code>Model</code>,
     * call <code>initializeModel</code> at the beginning of your
     * method, then register <code>Proxy</code>s.</P>
     *
     * <P>Note: This method is <i>rarely</i> overridden; in practice you are more
     * likely to use a <code>Command</code> to create and register <code>Proxy</code>s
     * with the <code>Model</code>, since <code>Proxy</code>s with mutable data will likely
     * need to send <code>Notification</code>s and thus will likely want to fetch a reference to
     * the <code>Facade</code> during their construction.</P>
     *
     * @param self Facade
     */
    void (*initializeModel)(Facade *self);

    /**
     * <P>Initialize the <code>View</code>.</P>
     *
     * <P>Called by the <code>initializeFacade</code> method.
     * Override this method in your subclass of <code>Facade</code>
     * if one or both of the following are true:</P>
     *
     * <UL>
     * <LI> You wish to initialize a different <code>View</code>.</LI>
     * <LI> You have <code>Observers</code> to register with the <code>View</code></LI>
     * </UL>
     *
     * <P>If you don't want to initialize a different <code>View</code>,
     * call <code>initializeView(self)</code> at the beginning of your
     * method, then register <code>Mediator</code> instances.</P>
     *
     * <P>Note: This method is <i>rarely</i> overridden; in practice you are more
     * likely to use a <code>Command</code> to create and register <code>Mediator</code>s
     * with the <code>View</code>, since <code>Mediator</code> instances will need to send
     * <code>Notification</code>s and thus will likely want to fetch a reference
     * to the <code>Facade</code> during their construction.</P>
     *
     * @param self Facade
     */
    void (*initializeView)(Facade *self);

    /**
     * <P>Register an <code>Command</code> with the <code>Controller</code> by Notification name.</P>
     *
     * @param self Facade
     * @param notificationName the name of the <code>Notification</code> to associate the <code>Command</code> with
     * @param factory factory that returns <code>Controller</code>
     */
    void (*registerCommand)(Facade *self, const char *notificationName, SimpleCommand *(*factory)(void));

    /**
     * <P>Remove a previously registered <code>Command</code> to <code>Notification</code> mapping from the Controller.</P>
     *
     * @param self Facade
     * @param notificationName the name of the <code>Notification</code> to remove the <code>Command</code> mapping for
     */
    void (*removeCommand)(Facade *self, const char *notificationName);

    /**
     * <P>Check if a Command is registered for a given Notification</P>
     *
     * @param self Facade
     * @param notificationName notification name
     * @return whether a Command is currently registered for the given <code>notificationName</code>.
     */
    bool (*hasCommand)(Facade *self, const char *notificationName);

    /**
     * <P>Register an <code>Proxy</code> with the <code>Model</code> by name.</P>
     *
     * @param self Facade
     * @param proxy the <code>Proxy</code> instance to be registered with the <code>Model</code>.
     */
    void (*registerProxy)(Facade *self, Proxy *proxy);

    /**
     * <P>Retrieve an <code>Proxy</code> from the <code>Model</code> by name.</P>
     *
     * @param self Facade
     * @param proxyName the name of the new to be retrieved.
     * @return the <code>Proxy</code> instance previously registered with the given <code>proxyName</code>.
     */
    Proxy *(*retrieveProxy)(Facade *self, const char *proxyName);

    /**
     * <P>Remove an <code>Proxy</code> from the <code>Model</code> by name.</P>
     *
     * @param self Facade
     * @param proxyName the <code>Proxy</code> to remove from the <code>Model</code>.
     * @return the <code>Proxy</code> that was removed from the <code>Model</code>
     */
    Proxy *(*removeProxy)(Facade *self, const char *proxyName);

    /**
     * <P>Check if a Proxy is registered</P>
     *
     * @param self Facade
     * @param proxyName new name
     * @return whether a Proxy is currently registered with the given <code>proxyName</code>.
     */
    bool (*hasProxy)(Facade *self, const char *proxyName);

    /**
     * <P>Register a <code>Mediator</code> with the <code>View</code>.</P>
     *
     * @param self Facade
     * @param mediator a reference to the <code>Mediator</code>
     */
    void (*registerMediator)(Facade *self, Mediator *mediator);

    /**
     * <P>Retrieve an <code>Mediator</code> from the <code>View</code>.</P>
     *
     * @param self Facade
     * @param mediatorName mediator name
     * @return the <code>Mediator</code> previously registered with the given <code>mediatorName</code>.
     */
    Mediator *(*retrieveMediator)(Facade *self, const char *mediatorName);

    /**
     * <P>Remove an <code>Mediator</code> from the <code>View</code>.</P>
     *
     * @param self Facade
     * @param mediatorName name of the <code>Mediator</code> to be removed.
     * @return the <code>Mediator</code> that was removed from the <code>View</code>
     */
    Mediator *(*removeMediator)(Facade *self, const char *mediatorName);

    /**
     * <P>Check if a Mediator is registered or not</P>
     *
     * @param self Facade
     * @param mediatorName mediator name
     * @return whether a Mediator is registered with the given <code>mediatorName</code>.
     */
    bool (*hasMediator)(Facade *self, const char *mediatorName);

    /**
     * <P>Create and send an <code>Notification</code>.</P>
     *
     * <P>Keeps us from having to construct new notification
     * instances in our implementation code.</P>
     *
     * @param self Facade
     * @param notificationName the name of the notification to send
     * @param body the body of the notification
     * @param type the type of the notification
     */
    void (*sendNotification)(Facade *self, const char *notificationName, void *body, char *type);

    /**
     * <P>Notify <code>Observer</code>s.</P>
     *
     * <P>This method is left public mostly for backward
     * compatibility, and to allow you to send custom
     * notification classes using the facade.</P>
     *
     * <P>Usually you should just call sendNotification
     * and pass the parameters, never having to
     * construct the notification yourself.</P>
     *
     * @param self Facade
     * @param notification the <code>Notification</code> to have the <code>View</code> notify <code>Observers</code> of.
     */
    void (*notifyObservers)(Facade *self, Notification *notification);

    /**
     * <P>Set the Multiton key for this facade instance.</P>
     *
     * <P>Not called directly, but instead from the
     * constructor when getInstance is invoked.
     * It is necessary to be public in order to
     * implement Notifier.</P>
     *
     * @param self Facade
     */
    void (*initializeNotifier)(Facade *self, const char *key);
};

struct $Facade {
    /** Constructor */
    Facade *(*new)(const char *key);

    /** Initializer */
    void (*init)(Facade *facade);

    /** Facade Multiton Factory method */
    Facade *(*getInstance)(const char *key, Facade *(*factory)(const char *));

    /** Check if a Core is registered or not */
    bool (*hasCore)(const char *key);

    /** Remove a Core. */
    void (*removeFacade)(const char *key);
};

const struct $Facade $Facade;

#endif //PUREMVC_FACADE_H
