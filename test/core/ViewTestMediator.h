#ifndef PUREMVC_VIEWTESTMEDIATOR_H
#define PUREMVC_VIEWTESTMEDIATOR_H

#include "interfaces/Mediator.h"

static char *ViewTestMediator_NAME = "ViewTestMediator";

typedef struct {
    Mediator mediator;
} ViewTestMediator;

ViewTestMediator *NewViewTestMediator(void *viewComponent);

#endif //PUREMVC_VIEWTESTMEDIATOR_H
