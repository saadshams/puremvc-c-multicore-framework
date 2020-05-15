#include "MacroCommandTestCommand.h"
#include "MacroCommandTestSub1Command.h"
#include "MacroCommandTestSub2Command.h"

/**
 * Initialize the MacroCommandTestCommand by adding
 * its 2 SubCommands.
 */
static void initializeMacroCommand(MacroCommand *self) {
    self->addSubCommand(self, (SimpleCommand *(*)()) NewMacroCommandTestSub1Command);
    self->addSubCommand(self, (SimpleCommand *(*)()) NewMacroCommandTestSub2Command);
}

/**
 * Constructor
 */
MacroCommand* NewMacroCommandTestCommand() {
    MacroCommand *self = NewMacroCommand();
    self->initializeMacroCommand = initializeMacroCommand;
    return self;
}