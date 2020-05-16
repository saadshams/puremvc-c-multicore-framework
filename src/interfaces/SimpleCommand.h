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
typedef struct SimpleCommand SimpleCommand;

struct SimpleCommand {
    struct Notifier *notifier;
    void (*execute)(SimpleCommand *self, Notification *notification);
};

/**
 * Initializer
 *
 * @param self
 */
void InitSimpleCommand(SimpleCommand *self);

/**
 * Constructor
 */
SimpleCommand *NewSimpleCommand();

/**
 * Destructor
 *
 * @param self
 */
void DeleteSimpleCommand(SimpleCommand *self);

#endif //PUREMVC_SIMPLECOMMAND_H
