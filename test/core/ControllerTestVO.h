#ifndef PUREMVC_CONTROLLERTESTVO_H
#define PUREMVC_CONTROLLERTESTVO_H

typedef struct ControllerTestVO ControllerTestVO;

struct ControllerTestVO {
    int input;
    int result;
};

ControllerTestVO *NewControllerTestVO(int input);

void DeleteControllerTestVO(ControllerTestVO *self);

#endif //PUREMVC_CONTROLLERTESTVO_H
