#ifndef PUREMVC_VIEWTESTMEDIATOR6_H
#define PUREMVC_VIEWTESTMEDIATOR6_H

#include "interfaces/Mediator.h"

typedef struct {
    Mediator mediator;
} ViewTestMediator6;

static char *ViewTestMediator6_NAME = "ViewTestMediator6";

ViewTestMediator6* NewViewTestMediator6(char *name, void *viewComponent);

#endif //PUREMVC_VIEWTESTMEDIATOR6_H