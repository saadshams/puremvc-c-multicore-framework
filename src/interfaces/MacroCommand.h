#ifndef PUREMVC_MACROCOMMAND_H
#define PUREMVC_MACROCOMMAND_H

#include "SimpleCommand.h"

typedef struct SubCommandNode SubCommandNode;

struct SubCommandNode {
    SimpleCommand *(*factory)();
    SubCommandNode *next;
};

typedef struct MacroCommand MacroCommand;

struct MacroCommand {
    struct Notifier *notifier;
    SubCommandNode *SubCommands;
    void (*initializeMacroCommand)(MacroCommand *self);
    void (*addSubCommand)(MacroCommand *self, SimpleCommand *(*factory)());
    void (*execute)(MacroCommand *self, Notification *notification);
};

void InitMacroCommand(MacroCommand *self);

MacroCommand *NewMacroCommand();

void DeleteMacroCommand(MacroCommand *self);

#endif //PUREMVC_MACROCOMMAND_H
