#include "interfaces/MacroCommand.h"
#include "interfaces/Notifier.h"
#include <stdio.h>
#include <stdlib.h>

// Construct a SubCommand Node for the SubCommands LinkedList
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

// Destruct the SubCommand Node
static void DeleteSubCommand(SubCommandNode *subCommandNode) {
    free(subCommandNode);
    subCommandNode = NULL;
}

static void initializeMacroCommand(MacroCommand *self) {

}

static void addSubCommand(MacroCommand *self, SimpleCommand *(*factory)()) {
    SubCommandNode **cursor = &self->SubCommands;
    while (*cursor)
        cursor = &(*cursor)->next;
    *cursor = NewSubCommandNode(factory);
}

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

    // releasing commands
    cursor = self->SubCommands;
    while (cursor) {
        SubCommandNode *next = cursor->next;
        DeleteSubCommand(cursor);
        cursor = next;
    }
    self->SubCommands = NULL;
}

MacroCommand *NewMacroCommand() {
    MacroCommand *macroCommand = malloc(sizeof(MacroCommand));
    if (macroCommand == NULL) goto exception;
    InitMacroCommand(macroCommand);
    return macroCommand;

    exception:
        fprintf(stderr, "MacroCommand allocation failed.\n");
        return NULL;
}

void InitMacroCommand(MacroCommand *macroCommand) {
    macroCommand->notifier = NewNotifier();
    macroCommand->SubCommands = NULL;
    macroCommand->initializeMacroCommand = initializeMacroCommand;
    macroCommand->addSubCommand = addSubCommand;
    macroCommand->execute = execute;
}

void DeleteMacroCommand(MacroCommand *macroCommand) {
    DeleteNotifier(macroCommand->notifier);
    free(macroCommand);
    macroCommand = NULL;
}
