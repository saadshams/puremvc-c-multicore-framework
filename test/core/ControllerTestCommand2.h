#ifndef PUREMVC_CONTROLLERTESTCOMMAND2_H
#define PUREMVC_CONTROLLERTESTCOMMAND2_H

#include "interfaces/SimpleCommand.h"

typedef struct ControllerTestCommand2 ControllerTestCommand2;

struct ControllerTestCommand2 {
    SimpleCommand simpleCommand;
};

ControllerTestCommand2 *NewControllerTestCommand2();

#endif //PUREMVC_CONTROLLERTESTCOMMAND2_H
