#ifndef PUREMVC_VIEWTESTMEDIATOR3_H
#define PUREMVC_VIEWTESTMEDIATOR3_H

#include "interfaces/Mediator.h"

/**
 * A Mediator struct used by ViewTest.
 *
 * @see ViewTest
 */
typedef struct {
    Mediator mediator;
} ViewTestMediator3;

/**
 * The Mediator name
 */
#define ViewTestMediator3_NAME "ViewTestMediator3"

ViewTestMediator3 *NewViewTestMediator3(void *viewComponent);

#endif //PUREMVC_VIEWTESTMEDIATOR3_H
