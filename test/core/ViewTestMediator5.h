#ifndef PUREMVC_VIEWTESTMEDIATOR5_H
#define PUREMVC_VIEWTESTMEDIATOR5_H

#include "interfaces/Mediator.h"

typedef struct {
    Mediator mediator;
} ViewTestMediator5;

static char *ViewTestMediator5_NAME = "ViewTestMediator5";

ViewTestMediator5* NewViewTestMediator5(void *viewComponent);

#endif //PUREMVC_VIEWTESTMEDIATOR5_H
