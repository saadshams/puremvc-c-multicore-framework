#ifndef PUREMVC_CONTROLLER_H
#define PUREMVC_CONTROLLER_H

#include "SimpleCommand.h"
#include "View.h"
#include <stdbool.h>

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
typedef struct CommandMap CommandMap;

typedef struct Controller Controller;

struct Controller {
    const char *multitonKey;
    View *view;
    CommandMap *commandMap;
    void (*initializeController)(Controller *self);
    void (*executeCommand)(Controller *self, Notification *notification);
    void (*registerCommand)(Controller *self, const char *notificationName, SimpleCommand *(*factory)());
    bool (*hasCommand)(Controller *self, const char *notificationName);
    void (*removeCommand)(Controller *self, const char *notificationName);
};

Controller *getControllerInstance(const char *key, Controller *(*factory)(const char *));

Controller *NewController(const char *key);

void InitController(Controller *controller);

void RemoveController(const char *key);

#endif //PUREMVC_CONTROLLER_H
