#pragma once

#include "puremvc/puremvc.h"
#include "ViewTest.h"

#define ViewTestMediator_NAME "Mediator"

struct IMediator *test_mediator_new(struct ViewComponent *component);
