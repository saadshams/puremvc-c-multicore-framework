#include "SimpleCommandTestVO.h"
#include <stdlib.h>

SimpleCommandTestVO *SimpleCommandTestVONew(int input) {
    SimpleCommandTestVO *simpleCommandTestVO = malloc(sizeof(SimpleCommandTestVO));
    simpleCommandTestVO->input = input;
    return simpleCommandTestVO;
}

void SimpleCommandTestVORelease(SimpleCommandTestVO *simpleCommandTestVO) {
    free(simpleCommandTestVO);
}