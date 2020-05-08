#ifndef PUREMVC_MACROCOMMANDTESTVO_H
#define PUREMVC_MACROCOMMANDTESTVO_H

typedef struct MacroCommandTestVO MacroCommandTestVO;

struct MacroCommandTestVO {
    int input;
    int result1;
    int result2;
};

MacroCommandTestVO *MacroCommandTestVONew(int input);

void MacroCommandTestVODelete(MacroCommandTestVO *self);

#endif //PUREMVC_MACROCOMMANDTESTVO_H
