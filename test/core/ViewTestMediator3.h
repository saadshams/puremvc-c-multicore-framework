#pragma once

#include "puremvc/puremvc.h"
#include "ViewTest.h"

#define ViewTestMediator3_NAME "Mediator3"

struct IMediator *test_mediator3_new(struct ViewTest *component, const char **error);
