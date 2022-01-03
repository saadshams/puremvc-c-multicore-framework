#ifndef PUREMVC_MEDIATOR_H
#define PUREMVC_MEDIATOR_H

#include "Notification.h"

/**
 * <P>The name of the <code>Mediator</code>.</P>
 *
 * <P>Typically, a <code>Mediator</code> will be written to serve
 * one specific control or group controls and so,
 * will not have a need to be dynamically named.</P>
 */
#define MEDIATOR_NAME "Mediator"

typedef struct Mediator Mediator;

struct Notifier;

/**
 * <P>A base <code>Mediator</code> implementation.</P>
 *
 * @see View
 */
struct Mediator {

    struct Notifier *notifier;

    const char *mediatorName;

    void *viewComponent;

    /** Get mediator name */
    const char *(*getMediatorName)(Mediator *self);

    /** Set the view component */
    void (*setViewComponent)(Mediator *self, void *viewComponent);

    /** Get the view component */
    void *(*getViewComponent)(Mediator *self);

    /**
     * <P>List the <code>Notification</code> names this
     * <code>Mediator</code> is interested in being notified of.</P>
     *
     * @param self Mediator
     * @return Array the list of <code>Notification</code> names
     */
    const char *const *(*listNotificationInterests)(Mediator *self);

    /**
     * <P>Handle <code>Notification</code>s.</P>
     *
     * <P>Typically this will be handled in a switch statement,
     * with one 'case' entry per <code>Notification</code>
     * the <code>Mediator</code> is interested in.</P>
     */
    void (*handleNotification)(Mediator *self, Notification *notification);

    /** <P>Called by the View when the Mediator is registered</P> */
    void (*onRegister)(Mediator *self);

    /** <P>Called by the View when the Mediator is removed</P> */
    void (*onRemove)(Mediator *self);
};

struct $Mediator {
    /** Constructor */
    Mediator *(*new)(const char *mediatorName, void *viewComponent);

    /** Initializer */
    void (*init)(Mediator *mediator, const char *mediatorName, void *viewComponent);

    /** Destructor */
    void (*delete)(Mediator *mediator);
};

const struct $Mediator $Mediator;

#endif //PUREMVC_MEDIATOR_H
