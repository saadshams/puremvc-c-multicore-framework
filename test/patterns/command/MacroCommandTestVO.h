#ifndef PUREMVC_MACROCOMMANDTESTVO_H
#define PUREMVC_MACROCOMMANDTESTVO_H

/**
 * A utility struct used by MacroCommandTest.
 *
 * @see MacroCommandTest MacroCommandTest
 * @see org.puremvc.c.multicore.patterns.command.MacroCommandTestCommand MacroCommandTestCommand
 * @see org.puremvc.c.multicore.patterns.command.MacroCommandTestSub1Command MacroCommandTestSub1Command
 * @see org.puremvc.c.multicore.patterns.command.MacroCommandTestSub2Command MacroCommandTestSub2Command
 */
typedef struct MacroCommandTestVO MacroCommandTestVO;

struct MacroCommandTestVO {
    int input;
    int result1;
    int result2;
};

#endif //PUREMVC_MACROCOMMANDTESTVO_H
