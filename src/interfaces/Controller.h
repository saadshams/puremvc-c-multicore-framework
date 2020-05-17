#ifndef PUREMVC_CONTROLLER_H
#define PUREMVC_CONTROLLER_H

#include "SimpleCommand.h"
#include "View.h"
#include <stdbool.h>

/**
 * <P>A Multiton <code>IController</code> implementation.</P>
 *
 * <P>In PureMVC, the <code>Controller</code> class follows the
 * 'Command and Controller' strategy, and assumes these
 * responsibilities:</P>
 *
 * <UL>
 * <LI> Remembering which <code>ICommand</code>
 * are intended to handle which <code>INotifications</code>.</LI>
 * <LI> Registering itself as an <code>IObserver</code> with
 * the <code>View</code> for each <code>INotification</code>
 * that it has an <code>ICommand</code> mapping for.</LI>
 * <LI> Creating a new instance of the proper <code>ICommand</code>
 * to handle a given <code>INotification</code> when notified by the <code>View</code>.</LI>
 * <LI> Calling the <code>ICommand</code>'s <code>execute</code>
 * method, passing in the <code>INotification</code>.</LI>
 * </UL>
 *
 * <P>Your application must register <code>ICommands</code> with the
 * Controller.</P>
 *
 * <P>The simplest way is to subclass <code>Facade</code>,
 * and use its <code>initializeController</code> method to add your
 * registrations.</P>
 *
 * @see View View
 * @see org.puremvc.c.multicore.patterns.observer.Observer Observer
 * @see org.puremvc.c.multicore.patterns.observer.Notification Notification
 * @see org.puremvc.c.multicore.patterns.command.SimpleCommand SimpleCommand
 * @see org.puremvc.c.multicore.patterns.command.MacroCommand MacroCommand
 */
typedef struct CommandMap CommandMap;

struct CommandMap {
    const char *name;
    SimpleCommand *(*factory)();
    CommandMap *next;
};

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
