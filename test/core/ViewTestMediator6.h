#pragma once

#include "puremvc/puremvc.h"
#include "ViewTest.h"

#define ViewTestMediator6_NAME "Mediator6"

struct IMediator *test_mediator6_new(const char *name, struct ViewTest *component, const char **error);
