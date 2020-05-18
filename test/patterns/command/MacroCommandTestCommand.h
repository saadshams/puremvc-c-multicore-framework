#ifndef PUREMVC_MACROCOMMANDTESTCOMMAND_H
#define PUREMVC_MACROCOMMANDTESTCOMMAND_H

#include "interfaces/Notification.h"
#include "interfaces/MacroCommand.h"

/**
 * A MacroCommand struct used by MacroCommandTest.
 *
 * @see MacroCommandTest
 * @see MacroCommandTestSub1Command
 * @see MacroCommandTestSub2Command
 * @see MacroCommandTestVO
 */
typedef struct MacroCommandTestCommand MacroCommandTestCommand;

MacroCommand* NewMacroCommandTestCommand();

#endif //PUREMVC_MACROCOMMANDTESTCOMMAND_H