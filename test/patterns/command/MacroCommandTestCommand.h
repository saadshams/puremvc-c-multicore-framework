#ifndef PUREMVC_MACROCOMMANDTESTCOMMAND_H
#define PUREMVC_MACROCOMMANDTESTCOMMAND_H

#include "interfaces/Notification.h"
#include "interfaces/MacroCommand.h"

/**
 * A MacroCommand struct used by MacroCommandTest.
 *
 * @see MacroCommandTest MacroCommandTest
 * @see org.puremvc.c.multicore.patterns.command.MacroCommandTestSub1Command MacroCommandTestSub1Command
 * @see org.puremvc.c.multicore.patterns.command.MacroCommandTestSub2Command MacroCommandTestSub2Command
 * @see MacroCommandTestVO MacroCommandTestVO
 */
typedef struct MacroCommandTestCommand MacroCommandTestCommand;

MacroCommand* NewMacroCommandTestCommand();

#endif //PUREMVC_MACROCOMMANDTESTCOMMAND_H