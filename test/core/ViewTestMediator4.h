#ifndef PUREMVC_VIEWTESTMEDIATOR4_H
#define PUREMVC_VIEWTESTMEDIATOR4_H

#include "interfaces/Mediator.h"
#include "ViewTest.h"

typedef struct {
    Mediator mediator;
} ViewTestMediator4;

static char *ViewTestMediator4_NAME = "ViewTestMediator4";

ViewTestMediator4 *NewTestMediator4(ViewTest *view);

#endif //PUREMVC_VIEWTESTMEDIATOR4_H
