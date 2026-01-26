#include "MacroCommandTestCommand2.h"
#include "MacroCommandTestSub3Command.h"

static void initializeMacroCommand(const struct IMacroCommand *self, const char **error) {
    self->addSubCommand(self, macro_command_test_sub3command_new, error);
}

struct IMacroCommand *macro_command_test_command2_new(const char **error) {
    struct IMacroCommand *command = puremvc_macro_command_new(error);
    command->initializeMacroCommand = initializeMacroCommand;
    return command;
}
