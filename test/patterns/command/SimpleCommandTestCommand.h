#ifndef PUREMVC_SIMPLECOMMANDTESTCOMMAND_H
#define PUREMVC_SIMPLECOMMANDTESTCOMMAND_H

#include "interfaces/SimpleCommand.h"
#include "interfaces/Notification.h"

/**
 * A SimpleCommand substruct used by SimpleCommandTest.
 *
 * @see SimpleCommandTest
 * @see SimpleCommandTestVO
 */
typedef struct SimpleCommandTestCommand SimpleCommandTestCommand;

struct SimpleCommandTestCommand {
    SimpleCommand simpleCommand;
};

SimpleCommandTestCommand *NewSimpleCommandTestCommand();

void DeleteSimpleCommandTestCommand(SimpleCommandTestCommand *self);

#endif //PUREMVC_SIMPLECOMMANDTESTCOMMAND_H
