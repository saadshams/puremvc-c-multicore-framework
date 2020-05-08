#ifndef PUREMVC_FACADETESTCOMMAND_H
#define PUREMVC_FACADETESTCOMMAND_H

#include "interfaces/SimpleCommand.h"

typedef struct {
    SimpleCommand simpleCommand;
} FacadeTestCommand;

FacadeTestCommand *NewFacadeTestCommand();

#endif //PUREMVC_FACADETESTCOMMAND_H
