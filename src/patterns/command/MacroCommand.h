/**
* @file MacroCommand.h
* @internal
* @brief MacroCommand Header
*
* @author Saad Shams <saad.shams@puremvc.org>
* @copyright BSD 3-Clause License
*/
#pragma once

#include "puremvc/IMacroCommand.h"

struct MacroCommand {
    struct IMacroCommand base;
    struct IArray *subCommands;
};
