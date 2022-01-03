#ifndef PUREMVC_MACROCOMMAND_H
#define PUREMVC_MACROCOMMAND_H

#include "List.h"
#include "SimpleCommand.h"

/**
 * <P>A base <code>Command</code> implementation that executes other <code>Command</code>s.</P>
 *
 * <P>A <code>MacroCommand</code> maintains an list of
 * <code>Command</code> Class references called <i>SubCommands</i>.</P>
 *
 * <P>When <code>execute</code> is called, the <code>MacroCommand</code>
 * instantiates and calls <code>execute</code> on each of its <i>SubCommands</i> turn.
 * Each <i>SubCommand</i> will be passed a reference to the original
 * <code>Notification</code> that was passed to the <code>MacroCommand</code>'s
 * <code>execute</code> method.</P>
 *
 * <P>Unlike <code>SimpleCommand</code>, your subclass
 * should not override <code>execute</code>, but instead, should
 * override the <code>initializeMacroCommand</code> method,
 * calling <code>addSubCommand</code> once for each <i>SubCommand</i>
 * to be executed.</P>
 *
 * @see Controller
 * @see Notification
 * @see SimpleCommand
 */
typedef struct MacroCommand MacroCommand;

struct MacroCommand {

    SimpleCommand simpleCommand;

    List *subCommands;

    /**
     * <P>Initialize the <code>MacroCommand</code>.</P>
     *
     * <P>In your subclass, override this method to
     * initialize the <code>MacroCommand</code>'s <i>SubCommand</i>
     * list with <code>Command</code> class references like
     * this:</P>
     *
     * ```c
     * static void initializeMacroCommand(MacroCommand *self) {
     * {
     *      self->addSubCommand(self, NewFirstCommand);
     *      self->addSubCommand(self, NewSecondCommand);
     *      self->addSubCommand(self, NewThirdCommand);
     * }
     * ```
     *
     * <P>Note that <i>SubCommand</i>s may be any <code>Command</code> implementor,
     * <code>MacroCommand</code>s or <code>SimpleCommands</code> are both acceptable.</P>
     *
     * @param self MacroCommand
     */
    void (*initializeMacroCommand)(MacroCommand *self);

    /**
     * <P>Add a <i>SubCommand</i>.</P>
     *
     * <P>The <i>SubCommands</i> will be called in First In/First Out (FIFO)
     * order.</P>
     *
     * @param self MacroCommand
     * @param factory a reference to the factory of the <code>Command</code>.
     */
    void (*addSubCommand)(MacroCommand *self, SimpleCommand *(*factory)(void));

    /**
     * <P>Execute this <code>MacroCommand</code>'s <i>SubCommands</i>.</P>
     *
     * <P>The <i>SubCommands</i> will be called in First In/First Out (FIFO)
     * order.</P>
     *
     * @param self MacroCommand
     * @param notification the <code>Notification</code> object to be passed to each <i>SubCommand</i>.
     */
    void (*execute)(MacroCommand *self, Notification *notification);
};

struct $MacroCommand {
    /** Constructor */
    MacroCommand *(*new)(void);

    /** Initializer */
    void (*init)(MacroCommand *macroCommand);

    /** Destructor */
    void (*delete)(MacroCommand *macroCommand);
};

const struct $MacroCommand $MacroCommand;

#endif //PUREMVC_MACROCOMMAND_H
