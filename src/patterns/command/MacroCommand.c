#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <collection/IArray.h>

#include "MacroCommand.h"

static void initializeMacroCommand(const struct IMacroCommand *self) {

}

static void addSubCommand(const struct IMacroCommand *self, struct ICommand *(*factory)(void)) {
    const struct MacroCommand *this = (struct MacroCommand *) self;
    this->subCommands->push(this->subCommands, factory);
}

static void execute(const struct ICommand *self, struct INotification *notification) {
    struct MacroCommand *this = (struct MacroCommand *) self;
    this->base.initializeMacroCommand((struct IMacroCommand *)this);

    while (this->subCommands->size(this->subCommands) > 0) {
        struct ICommand *(*factory)(void) = (struct ICommand *(*)(void))this->subCommands->shift(this->subCommands);
        struct ICommand *command = factory();
        command->notifier->initializeNotifier(command->notifier, self->notifier->getMultitonKey(self->notifier));
        command->execute(command, notification);
        puremvc_simple_command_free(&command);
    }

    collection_array_free(&this->subCommands);
}

static struct MacroCommand *init(struct MacroCommand *command) {
    if (command == NULL) return NULL;
    command->base.initializeMacroCommand = initializeMacroCommand;
    command->base.addSubCommand = addSubCommand;
    command->base.command.execute = execute;
    return command;
}

static struct MacroCommand *alloc() {
    struct MacroCommand *command = malloc(sizeof(struct MacroCommand));
    if (command == NULL) {
        fprintf(stderr, "MacroCommand allocation failed.\n");
        return NULL;
    }

    memset(command, 0, sizeof(struct MacroCommand));

    command->base.command.notifier = puremvc_notifier_new();
    command->subCommands = collection_array_new();
    return command;
}

struct IMacroCommand *puremvc_macro_command_new() {
    return (struct IMacroCommand *) init(alloc());
}

void puremvc_macro_command_free(struct IMacroCommand **command) {
    if (command == NULL || *command == NULL) return;

    struct MacroCommand *this = (struct MacroCommand *)*command;
    puremvc_notifier_free(&this->base.command.notifier);
    free(this);

    *command = NULL;
}
