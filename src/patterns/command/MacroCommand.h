#pragma once

#include "puremvc/IMacroCommand.h"

struct MacroCommand {
    struct IMacroCommand base;
    struct IArray *subCommands;
};
