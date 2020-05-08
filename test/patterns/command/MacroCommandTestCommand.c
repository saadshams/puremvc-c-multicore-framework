#include "MacroCommandTestCommand.h"
#include "MacroCommandTestSub1Command.h"
#include "MacroCommandTestSub2Command.h"

void (*_initializeMacroCommand)(MacroCommand *self); // base version

static void initializeMacroCommand(MacroCommand *self) {
    _initializeMacroCommand(self);
    self->addSubCommand(self, (SimpleCommand *(*)()) MacroCommandTestSub1CommandNew);
    self->addSubCommand(self, (SimpleCommand *(*)()) MacroCommandTestSub2CommandNew);
}

MacroCommand* MacroCommandTestCommandNew() {
    MacroCommand *self = NewMacroCommand();
    _initializeMacroCommand = self->initializeMacroCommand;
    self->initializeMacroCommand = initializeMacroCommand;
    return self;
}

MacroCommand* MacroCommandTestCommandNew2() {
    MacroCommand *macroCommand = NewMacroCommand();
    _initializeMacroCommand = macroCommand->initializeMacroCommand; // retain base initializeMacroCommand
    macroCommand->initializeMacroCommand = initializeMacroCommand; // override initializeMacroCommand
    return macroCommand;
}