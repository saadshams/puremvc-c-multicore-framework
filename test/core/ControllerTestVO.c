#include "ControllerTestVO.h"
#include <stdlib.h>

static void release(ControllerTestVO *self) {
    free(self);
}

ControllerTestVO *ControllerTestVONew(int input) {
    ControllerTestVO *self = malloc(sizeof(ControllerTestVO));
    self->input = input;
    self->result = 0;
    self->release = release;
    return self;
}