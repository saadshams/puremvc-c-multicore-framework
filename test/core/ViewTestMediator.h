#ifndef PUREMVC_VIEWTESTMEDIATOR_H
#define PUREMVC_VIEWTESTMEDIATOR_H

#include "interfaces/Mediator.h"

/**
 * A Mediator struct used by ViewTest.
 *
 * @see ViewTest
 */
typedef struct {
    Mediator mediator;
} ViewTestMediator;

/**
 * The Mediator name
 */
#define ViewTestMediator_NAME "ViewTestMediator"

ViewTestMediator *NewViewTestMediator(void *viewComponent);

#endif //PUREMVC_VIEWTESTMEDIATOR_H
