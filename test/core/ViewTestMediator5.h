#ifndef PUREMVC_VIEWTESTMEDIATOR5_H
#define PUREMVC_VIEWTESTMEDIATOR5_H

#include "interfaces/Mediator.h"

/**
 * A Mediator struct used by ViewTest.
 *
 * @see ViewTest ViewTest
 */
typedef struct {
    Mediator mediator;
} ViewTestMediator5;

/**
 * The Mediator name
 */
#define ViewTestMediator5_NAME "ViewTestMediator5"

ViewTestMediator5* NewViewTestMediator5(void *viewComponent);

#endif //PUREMVC_VIEWTESTMEDIATOR5_H
