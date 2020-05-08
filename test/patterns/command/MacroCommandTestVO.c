#include "MacroCommandTestVO.h"
#include <stdlib.h>

MacroCommandTestVO *MacroCommandTestVONew(int input) {
    MacroCommandTestVO *self = malloc(sizeof(MacroCommandTestVO));
    self->input = input;
    return self;
}

void MacroCommandTestVODelete(MacroCommandTestVO *self) {
    free(self);
}
