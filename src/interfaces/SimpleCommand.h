#ifndef PUREMVC_SIMPLECOMMAND_H
#define PUREMVC_SIMPLECOMMAND_H

#include "Notification.h"

struct Notifier;

/**
 * <P>A base <code>ICommand</code> implementation.</P>
 *
 * <P>Your subclass should override the <code>execute</code>
 * method where your business logic will handle the <code>INotification</code>.</P>
 *
 * @see org.puremvc.c.multicore.core.Controller Controller
 * @see org.puremvc.c.multicore.patterns.observer.Notification Notification
 * @see MacroCommand MacroCommand
 */

/// <P>A base <code>ICommand</code> implementation.</P>
typedef struct SimpleCommand SimpleCommand;

struct SimpleCommand {
    struct Notifier *notifier;

    /**
     * <P>Fulfill the use-case initiated by the given <code>INotification</code>.</P>
     *
     * <P>In the Command Pattern, an application use-case typically
     * begins with some user action, which results in an <code>INotification</code> being broadcast, which
     * is handled by business logic in the <code>execute</code> method of an
     * <code>ICommand</code>.</P>
     *
     * @param self *SimpleCommand
     * @param notification *Notification
     */
    void (*execute)(SimpleCommand *self, Notification *notification);
};

/**
*
*
*
*
*
*
*
* @param self
* @param notification the <code>INotification</code> to handle.
*/


/**
 * Constructor
 */
SimpleCommand *NewSimpleCommand();

/**
 * Initializer
 *
 * @param simpleCommand
 */
void InitSimpleCommand(SimpleCommand *simpleCommand);

/**
 * Destructor
 *
 * @param simpleCommand
 */
void DeleteSimpleCommand(SimpleCommand *simpleCommand);

#endif //PUREMVC_SIMPLECOMMAND_H
