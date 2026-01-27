#pragma once

#include "puremvc/puremvc.h"

#define ON_REGISTER_CALLED "onRegister Called"
#define ON_REMOVE_CALLED "onRemove Called"

struct IProxy *model_test_proxy_new(const char *name, void *data, const char **error);

