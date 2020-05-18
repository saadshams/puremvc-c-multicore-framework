#ifndef PUREMVC_VIEWTESTMEDIATOR4_H
#define PUREMVC_VIEWTESTMEDIATOR4_H

#include "interfaces/Mediator.h"
#include "ViewTest.h"

/**
 * A Mediator struct used by ViewTest.
 *
 * @see ViewTest
 */
typedef struct {
    Mediator mediator;
} ViewTestMediator4;

/**
 * The Mediator name
 */
#define ViewTestMediator4_NAME "ViewTestMediator4"

ViewTestMediator4 *NewTestMediator4(ViewTest *view);

#endif //PUREMVC_VIEWTESTMEDIATOR4_H
