#pragma once

#include "puremvc/puremvc.h"
#include "ViewTest.h"

#define ViewTestMediator4_NAME "Mediator4"

struct IMediator *test_mediator4_new(struct ViewTest *component, const char **error);
