#include "MacroCommandTestCommand.h"
#include "MacroCommandTestSub1Command.h"
#include "MacroCommandTestSub2Command.h"

static void initializeMacroCommand(const struct IMacroCommand *self, const char **error) {
    self->addSubCommand(self, macro_command_test_sub1command_new, error);
    self->addSubCommand(self, macro_command_test_sub2command_new, error);
}

struct IMacroCommand *macro_command_test_command_new(const char **error) {
    struct IMacroCommand *command = puremvc_macro_command_new(error);
    command->initializeMacroCommand = initializeMacroCommand;
    return command;
}
