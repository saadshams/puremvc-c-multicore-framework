#include "ControllerTestVO.h"
#include <stdlib.h>

ControllerTestVO *NewControllerTestVO(int input) {
    ControllerTestVO *self = malloc(sizeof(ControllerTestVO));
    self->input = input;
    self->result = 0;
    return self;
}

void DeleteControllerTestVO(ControllerTestVO *self) {
    free(self);
}