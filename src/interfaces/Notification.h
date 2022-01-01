#ifndef PUREMVC_NOTIFICATION_H
#define PUREMVC_NOTIFICATION_H

/**
 * <P>A base <code>Notification</code> implementation.</P>
 *
 * <P>PureMVC does not rely upon underlying event models such
 * as the one provided with Flash, and ActionScript 3 does
 * not have an inherent event model.</P>
 *
 * <P>The Observer Pattern as implemented within PureMVC exists
 * to support event-driven communication between the
 * application and the actors of the MVC triad.</P>
 *
 * <P>Notifications are not meant to be a replacement for Events
 * in Flex/Flash/Apollo. Generally, <code>Mediator</code> implementors
 * place event listeners on their view components, which they
 * then handle in the usual way. This may lead to the broadcast of <code>Notification</code>s to
 * trigger <code>Command</code>s or to communicate with other <code>Mediators</code>. <code>Proxy</code> and <code>Command</code>
 * instances communicate with each other and <code>Mediator</code>s
 * by broadcasting <code>Notification</code>s.</P>
 *
 * <P>A key difference between Flash <code>Event</code>s and PureMVC
 * <code>Notification</code>s is that <code>Event</code>s follow the
 * 'Chain of Responsibility' pattern, 'bubbling' up the display hierarchy
 * until some parent component handles the <code>Event</code>, while
 * PureMVC <code>Notification</code>s follow a 'Publish/Subscribe'
 * pattern. PureMVC classes need not be related to each other in a
 * parent/child relationship in order to communicate with one another
 * using <code>Notification</code>s.</P>
 *
 * @see Observer Observer
 */
typedef struct Notification Notification;

struct Notification {

    const char *name;

    void *body;

    char *type;

    /**
     * Get the name of the <code>Notification</code> instance.
     *
     * @return the name of the <code>Notification</code> instance.
     */
    const char *(*getName)(Notification *self);

    /**
     * Get the body of the <code>Notification</code> instance.
     *
     * @return the body object.
     */
    void *(*getBody)(Notification *self);

    /**
     * Set the body of the <code>Notification</code> instance.
     */
    void (*setBody)(Notification *self, void *body);

    /**
     * Get the type of the <code>Notification</code> instance.
     *
     * @return the type
     */
    char *(*getType)(Notification *self);

    /**
     * Set the type of the <code>Notification</code> instance.
     */
    void (*setType)(Notification *self, char *type);

    /**
     * Get the string representation of the <code>Notification</code> instance.
     *
     * @return the string representation of the <code>Notification</code> instance.
     */
    const char *(*toString)(Notification *self);
};

struct $Notification {
    /**
     * Constructor.
     *
     * @param name name of the <code>Notification</code> instance. (required)
     * @param body the <code>Notification</code> body. (optional)
     * @param type the type of the <code>Notification</code> (optional)
     */
    Notification *(*new)(const char *name, void *body, char *type);

    /** Initializer */
    void (*init)(Notification *notification, const char *name, void *body, char *type);

    /** Destructor */
    void (*delete)(Notification *notification);
};

const struct $Notification $Notification;

#endif //PUREMVC_NOTIFICATION_H
