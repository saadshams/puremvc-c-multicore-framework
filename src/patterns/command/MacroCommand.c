/**
* @file MacroCommand.c
* @internal
* @brief MacroCommand Implementation
*
* @author Saad Shams <saad.shams@puremvc.org>
* @copyright BSD 3-Clause License
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <collection/IArray.h>

#include "MacroCommand.h"

static void initializeMacroCommand(const struct IMacroCommand *self) {

}

static void addSubCommand(const struct IMacroCommand *self, struct ICommand *(*factory)(const char **error)) {
    const struct MacroCommand *this = (struct MacroCommand *) self;
    this->subCommands->push(this->subCommands, factory);
}

static void execute(const struct ICommand *self, struct INotification *notification, const char **error) {
    struct MacroCommand *this = (struct MacroCommand *) self;

    this->base.initializeMacroCommand((struct IMacroCommand *) this);
    while (this->subCommands->size(this->subCommands) > 0) {
        struct ICommand *(*factory)(const char **error) = (struct ICommand *(*)(const char **)) this->subCommands->shift(this->subCommands);
        struct ICommand *command = factory(error);
        if (*error != NULL) return;

        command->notifier->initializeNotifier(command->notifier, self->notifier->getKey(self->notifier), error);
        if (*error != NULL) return puremvc_simple_command_free(&command);

        command->execute(command, notification, error);
        if (*error != NULL) return puremvc_simple_command_free(&command);

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

static struct MacroCommand *alloc(const char **error) {
    struct MacroCommand *command = malloc(sizeof(struct MacroCommand));
    if (command == NULL) return *error = "[PureMVC::MacroCommand::alloc] Error: Failed to allocate MacroCommand.", NULL;

    memset(command, 0, sizeof(struct MacroCommand));

    command->base.command.notifier = puremvc_notifier_new(error);
    if (command->base.command.notifier == NULL) return free(command), NULL;

    command->subCommands = collection_array_new();
    return command;
}

struct IMacroCommand *puremvc_macro_command_new(const char **error) {
    return (struct IMacroCommand *) init(alloc(error));
}

void puremvc_macro_command_free(struct IMacroCommand **command) {
    if (command == NULL || *command == NULL) return;
    struct MacroCommand *this = (struct MacroCommand *) *command;

    puremvc_notifier_free(&this->base.command.notifier);
    collection_array_free(&this->subCommands);

    free(this);
    *command = NULL;
}
