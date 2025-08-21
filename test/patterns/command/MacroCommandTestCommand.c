#include "MacroCommandTestCommand.h"
#include "MacroCommandTestSub1Command.h"
#include "MacroCommandTestSub2Command.h"

static void initializeMacroCommand(const struct IMacroCommand *self) {
    self->addSubCommand(self, macro_command_test_sub1command_new);
    self->addSubCommand(self, macro_command_test_sub2command_new);
}

struct IMacroCommand *macro_command_test_command_new() {
    struct IMacroCommand *command = puremvc_macro_command_new();
    command->initializeMacroCommand = initializeMacroCommand;
    return command;
}
