#include "interfaces/MacroCommand.h"
#include "interfaces/Notifier.h"
#include <stdio.h>
#include <stdlib.h>

static SubCommandNode *NewSubCommandNode(SimpleCommand *(*factory)()) {
    SubCommandNode *self = malloc(sizeof(SubCommandNode));
    self->factory = factory;
    self->next = NULL;
    return self;
}

static void DeleteSubCommand(SubCommandNode *self) {
    free(self);
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

    cursor = self->SubCommands;
    while (cursor) {
        SubCommandNode *next = cursor->next;
        DeleteSubCommand(cursor);
        cursor = next;
    }
    self->SubCommands = NULL;
}

void InitMacroCommand(MacroCommand *self) {
    if (self) {
        self->notifier = NewNotifier();
        self->SubCommands = NULL;
        self->initializeMacroCommand = initializeMacroCommand;
        self->addSubCommand = addSubCommand;
        self->execute = execute;
    }
}

MacroCommand *NewMacroCommand() {
    MacroCommand *self = malloc(sizeof(MacroCommand));
    InitMacroCommand(self);
    return self;
}

void DeleteMacroCommand(MacroCommand *self) {
    free(self);
}
