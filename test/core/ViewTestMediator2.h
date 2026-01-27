#pragma once

#include "puremvc/puremvc.h"
#include "ViewTest.h"

#define ViewTestMediator2_NAME "Mediator2"

struct IMediator *test_mediator2_new(struct ViewTest *component, const char **error);
