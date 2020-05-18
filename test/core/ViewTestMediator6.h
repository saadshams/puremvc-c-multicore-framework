#ifndef PUREMVC_VIEWTESTMEDIATOR6_H
#define PUREMVC_VIEWTESTMEDIATOR6_H

#include "interfaces/Mediator.h"

/**
 * A Mediator class used by ViewTest.
 *
 * @see ViewTest
 */
typedef struct {
    Mediator mediator;
} ViewTestMediator6;

/**
 * The Mediator base name
 */
#define ViewTestMediator6_NAME "ViewTestMediator6"

ViewTestMediator6* NewViewTestMediator6(char *name, void *viewComponent);

#endif //PUREMVC_VIEWTESTMEDIATOR6_H