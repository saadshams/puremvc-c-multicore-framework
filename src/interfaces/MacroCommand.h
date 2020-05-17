#ifndef PUREMVC_MACROCOMMAND_H
#define PUREMVC_MACROCOMMAND_H

#include "SimpleCommand.h"

/**
 * A LinkedList of SimpleCommand factory nodes
 */
typedef struct SubCommandNode SubCommandNode;

struct SubCommandNode {
    SimpleCommand *(*factory)();
    SubCommandNode *next;
};

/**
 * <P>A base <code>ICommand</code> implementation that executes other <code>ICommand</code>s.</P>
 *
 * <P>A <code>MacroCommand</code> maintains an list of
 * <code>ICommand</code> Class references called <i>SubCommands</i>.</P>
 *
 * <P>When <code>execute</code> is called, the <code>MacroCommand</code>
 * instantiates and calls <code>execute</code> on each of its <i>SubCommands</i> turn.
 * Each <i>SubCommand</i> will be passed a reference to the original
 * <code>INotification</code> that was passed to the <code>MacroCommand</code>'s
 * <code>execute</code> method.</P>
 *
 * <P>Unlike <code>SimpleCommand</code>, your subclass
 * should not override <code>execute</code>, but instead, should
 * override the <code>initializeMacroCommand</code> method,
 * calling <code>addSubCommand</code> once for each <i>SubCommand</i>
 * to be executed.</P>
 *
 * @see org.puremvc.c.multicore.core.Controller Controller
 * @see org.puremvc.c.multicore.patterns.observer.Notification Notification
 * @see org.puremvc.c.multicore.patterns.command.SimpleCommand SimpleCommand
 */
typedef struct MacroCommand MacroCommand;

struct MacroCommand {
    struct Notifier *notifier;
    SubCommandNode *SubCommands;
    void (*initializeMacroCommand)(MacroCommand *self);
    void (*addSubCommand)(MacroCommand *self, SimpleCommand *(*factory)());
    void (*execute)(MacroCommand *self, Notification *notification);
};

/**
 * Initializer
 *
 * @param macroCommand
 */
void InitMacroCommand(MacroCommand *macroCommand);

/**
 * Constructor
 */
MacroCommand *NewMacroCommand();

/**
 * Destructor
 *
 * @param macroCommand
 */
void DeleteMacroCommand(MacroCommand *macroCommand);

#endif //PUREMVC_MACROCOMMAND_H
