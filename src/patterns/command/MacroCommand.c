#include "interfaces/MacroCommand.h"
#include "interfaces/Notifier.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * Construct a SubCommand Node for the SubCommands LinkedList
 *
 * @param factory
 */
static SubCommandNode *NewSubCommandNode(SimpleCommand *(*factory)()) {
    SubCommandNode *self = malloc(sizeof(SubCommandNode));
    if (self == NULL) goto exception;
    self->factory = factory;
    self->next = NULL;
    return self;

    exception:
        fprintf(stderr, "SubCommand allocation failed.\n");
        return NULL;
}

/**
 * Destruct the SubCommand Node
 * @param subCommandNode
 */
static void DeleteSubCommand(SubCommandNode *subCommandNode) {
    free(subCommandNode);
    subCommandNode = NULL;
}

/**
 * <P>Initialize the <code>MacroCommand</code>.</P>
 *
 * <P>In your subclass, override this method to
 * initialize the <code>MacroCommand</code>'s <i>SubCommand</i>
 * list with <code>ICommand</code> class references like
 * this:</P>
 *
 * <code>
 * static void initializeMacroCommand(MacroCommand *self) {
 * {
 *      self->addSubCommand(self, NewFirstCommand);
 *      self->addSubCommand(self, NewSecondCommand);
 *      self->addSubCommand(self, NewThirdCommand);
 * }
 * </code>
 *
 * <P>Note that <i>SubCommand</i>s may be any <code>ICommand</code> implementor,
 * <code>MacroCommand</code>s or <code>SimpleCommands</code> are both acceptable.</P>
 *
 * @param self
 */
static void initializeMacroCommand(MacroCommand *self) {

}

/**
 * <P>Add a <i>SubCommand</i>.</P>
 *
 * <P>The <i>SubCommands</i> will be called in First In/First Out (FIFO)
 * order.</P>
 *
 * @param self
 * @param factory a reference to the factory of the <code>ICommand</code>.
 */
static void addSubCommand(MacroCommand *self, SimpleCommand *(*factory)()) {
    SubCommandNode **cursor = &self->SubCommands;
    while (*cursor)
        cursor = &(*cursor)->next;

    *cursor = NewSubCommandNode(factory);
}

/**
 * <P>Execute this <code>MacroCommand</code>'s <i>SubCommands</i>.</P>
 *
 * <P>The <i>SubCommands</i> will be called in First In/First Out (FIFO)
 * order.</P>
 *
 * @param self
 * @param notification the <code>INotification</code> object to be passsed to each <i>SubCommand</i>.
 */
static void execute(MacroCommand *self, Notification *notification) {
    self->initializeMacroCommand(self);
    SubCommandNode *cursor = self->SubCommands;

    while (cursor) {
        SimpleCommand *commandInstance = cursor->factory();
        commandInstance->notifier->initializeNotifier(commandInstance->notifier, self->notifier->multitonKey);
        commandInstance->execute(commandInstance, notification);
        DeleteSimpleCommand(commandInstance);
        cursor = cursor->next;
    }

    // releasing memory
    cursor = self->SubCommands;
    while (cursor) {
        SubCommandNode *next = cursor->next;
        DeleteSubCommand(cursor);
        cursor = next;
    }
    self->SubCommands = NULL;
}

/**
 * Initializer
 *
 * @param macroCommand
 */
void InitMacroCommand(MacroCommand *macroCommand) {
    macroCommand->notifier = NewNotifier();
    macroCommand->SubCommands = NULL;
    macroCommand->initializeMacroCommand = initializeMacroCommand;
    macroCommand->addSubCommand = addSubCommand;
    macroCommand->execute = execute;
}

/**
 * Constructor
 */
MacroCommand *NewMacroCommand() {
    MacroCommand *macroCommand = malloc(sizeof(MacroCommand));
    if (macroCommand == NULL) goto exception;
    InitMacroCommand(macroCommand);
    return macroCommand;

    exception:
        fprintf(stderr, "MacroCommand allocation failed.\n");
        return NULL;
}

/**
 * Destructor
 *
 * @param macroCommand
 */
void DeleteMacroCommand(MacroCommand *macroCommand) {
    DeleteNotifier(macroCommand->notifier);
    free(macroCommand);
    macroCommand = NULL;
}
