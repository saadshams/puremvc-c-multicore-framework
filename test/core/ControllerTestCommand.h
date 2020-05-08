#ifndef PUREMVC_CONTROLLERTESTCOMMAND_H
#define PUREMVC_CONTROLLERTESTCOMMAND_H

#include "interfaces/SimpleCommand.h"

typedef struct ControllerTestCommand ControllerTestCommand;

struct ControllerTestCommand {
    SimpleCommand simpleCommand;
};

ControllerTestCommand *NewControllerTestCommand();

#endif //PUREMVC_CONTROLLERTESTCOMMAND_H