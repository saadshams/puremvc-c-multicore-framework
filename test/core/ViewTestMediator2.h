#ifndef PUREMVC_VIEWTESTMEDIATOR2_H
#define PUREMVC_VIEWTESTMEDIATOR2_H

#include "interfaces/Mediator.h"

typedef struct {
    Mediator mediator;
} ViewTestMediator2;

static char *ViewTestMediator2_NAME = "ViewTestMediator2";

ViewTestMediator2* ViewTestMediator2New(void *viewComponent);

#endif //PUREMVC_VIEWTESTMEDIATOR2_H
