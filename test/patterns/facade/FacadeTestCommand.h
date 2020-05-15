#ifndef PUREMVC_FACADETESTCOMMAND_H
#define PUREMVC_FACADETESTCOMMAND_H

#include "interfaces/SimpleCommand.h"

/**
 * A SimpleCommand substruct used by FacadeTest.
 *
 * @see FacadeTest FacadeTest
 * @see FacadeTestVO FacadeTestVO
 */
typedef struct {
    SimpleCommand simpleCommand;
} FacadeTestCommand;

FacadeTestCommand *NewFacadeTestCommand();

#endif //PUREMVC_FACADETESTCOMMAND_H
