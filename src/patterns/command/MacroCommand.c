#include "interfaces/MacroCommand.h"
#include "interfaces/Notifier.h"
#include <stdio.h>
#include <stdlib.h>

static void initializeMacroCommand(MacroCommand *self) {

}

static void addSubCommand(MacroCommand *self, SimpleCommand *(*factory)(void)) {
    $List.addLast(&self->subCommands, factory);
}

static void execute(MacroCommand *self, Notification *notification) {
    self->initializeMacroCommand(self);

    while ($List.size(&self->subCommands) > 0) {
        SimpleCommand *(*factory)() = $List.shift(&self->subCommands);
        SimpleCommand *commandInstance = factory();
        commandInstance->notifier->initializeNotifier(commandInstance->notifier,
                                                      self->simpleCommand.notifier->multitonKey);
        commandInstance->execute(commandInstance, notification);
        $SimpleCommand.delete(commandInstance);
    }

    self->subCommands = NULL;
}

static void init(MacroCommand *macroCommand) {
    macroCommand->simpleCommand.notifier = $Notifier.new();
    macroCommand->subCommands = NULL;
    macroCommand->initializeMacroCommand = initializeMacroCommand;
    macroCommand->addSubCommand = addSubCommand;
    macroCommand->simpleCommand.execute = (void (*)(SimpleCommand *, Notification *)) execute;
    macroCommand->execute = execute;
}

static MacroCommand *new(void) {
    MacroCommand *macroCommand = malloc(sizeof(MacroCommand));
    if (macroCommand == NULL) goto exception;
    $MacroCommand.init(macroCommand);
    return macroCommand;

    exception:
    fprintf(stderr, "MacroCommand allocation failed.\n");
    return NULL;
}

static void delete(MacroCommand *macroCommand) {
    $Notifier.delete(macroCommand->simpleCommand.notifier);
    free(macroCommand);
    macroCommand = NULL;
}

const struct $MacroCommand $MacroCommand = {.new = new, .init = init, .delete = delete};
