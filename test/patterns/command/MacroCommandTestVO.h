#ifndef PUREMVC_MACROCOMMANDTESTVO_H
#define PUREMVC_MACROCOMMANDTESTVO_H

/**
 * A utility struct used by MacroCommandTest.
 *
 * @see MacroCommandTest
 * @see MacroCommandTestCommand
 * @see MacroCommandTestSub1Command
 * @see MacroCommandTestSub2Command
 */
typedef struct MacroCommandTestVO MacroCommandTestVO;

struct MacroCommandTestVO {
    int input;
    int result1;
    int result2;
};

#endif //PUREMVC_MACROCOMMANDTESTVO_H
