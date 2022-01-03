#include "MacroCommandTestCommand.h"
#include "MacroCommandTestSub1Command.h"
#include "MacroCommandTestSub2Command.h"

/**
 * Initialize the MacroCommandTestCommand by adding
 * its 2 SubCommands.
 */
static void initializeMacroCommand(MacroCommand *self) {
    self->addSubCommand(self, NewMacroCommandTestSub1Command);
    self->addSubCommand(self, NewMacroCommandTestSub2Command);
}

/**
 * Constructor
 */
MacroCommand *NewMacroCommandTestCommand() {
    MacroCommand *self = $MacroCommand.new();
    self->initializeMacroCommand = initializeMacroCommand;
    return self;
}
