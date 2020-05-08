#ifndef PUREMVC_SIMPLECOMMANDTESTVO_H
#define PUREMVC_SIMPLECOMMANDTESTVO_H

typedef struct SimpleCommandTestVO SimpleCommandTestVO;

struct SimpleCommandTestVO {
    int input;
    int result;
};

SimpleCommandTestVO *SimpleCommandTestVONew(int input);

void SimpleCommandTestVORelease(SimpleCommandTestVO *simpleCommandTestVo);

#endif //PUREMVC_SIMPLECOMMANDTESTVO_H
