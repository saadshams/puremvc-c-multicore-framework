#ifndef PUREMVC_CONTROLLERTESTVO_H
#define PUREMVC_CONTROLLERTESTVO_H

typedef struct ControllerTestVO ControllerTestVO;

struct ControllerTestVO {
    int input;
    int result;
    void (*release)(ControllerTestVO *self);
};

ControllerTestVO *ControllerTestVONew(int input);

#endif //PUREMVC_CONTROLLERTESTVO_H
