#ifndef PUREMVC_NOTIFICATION_H
#define PUREMVC_NOTIFICATION_H

/**
 * <P>A base <code>INotification</code> implementation.</P>
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
 * in Flex/Flash/Apollo. Generally, <code>IMediator</code> implementors
 * place event listeners on their view components, which they
 * then handle in the usual way. This may lead to the broadcast of <code>Notification</code>s to
 * trigger <code>ICommand</code>s or to communicate with other <code>IMediators</code>. <code>IProxy</code> and <code>ICommand</code>
 * instances communicate with each other and <code>IMediator</code>s
 * by broadcasting <code>INotification</code>s.</P>
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
    const char *(*getName)(Notification *self);
    void *(*getBody)(Notification *self);
    void (*setBody)(Notification *self, void *body);
    char *(*getType)(Notification *self);
    void (*setType)(Notification *self, char *type);
};

/**
 * Initializer
 *
 * @param notification
 * @param name
 * @param body
 * @param type
 */
void InitNotification(Notification *notification, const char *name, void *body, char *type);

/**
 * Constructor
 *
 * @param name
 * @param body
 * @param type
 */
Notification *NewNotification(const char *name, void *body, char *type);

/**
 * Destructor
 *
 * @param notification
 */
void DeleteNotification(Notification *notification);

#endif //PUREMVC_NOTIFICATION_H
