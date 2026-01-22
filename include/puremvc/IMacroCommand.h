#pragma once

#include "ICommand.h"

/**
 * @struct IMacroCommand
 * @brief Represents a MacroCommand in the PureMVC framework.
 *
 * This structure extends ICommand and allows sequencing of multiple sub-commands.
 * It provides hooks to initialize the macro command and to add sub-commands dynamically.
 */
struct IMacroCommand {
    struct ICommand command;

    /**
     * @brief Initializes the macro command.
     *
     * Typically, overridden to add the sub-commands to execute.
     *
     * @param self Pointer to this macro command instance.
     */
    void (*initializeMacroCommand)(const struct IMacroCommand *self);

    /**
     * @brief Adds a sub-command to this macro command.
     *
     * @param self Pointer to this macro command instance.
     * @param factory Function pointer returning a new ICommand instance.
     */
    void (*addSubCommand)(const struct IMacroCommand *self, struct ICommand *(*factory)());
};

/**
 * @brief Creates a new macro command.
 *
 * Allocates memory and initializes the macro command structure.
 *
 * @param error Out-param for static error string on failure (NULL on success).
 * @return A pointer to the initialized IMacroCommand, or NULL on failure.
 */
struct IMacroCommand *puremvc_macro_command_new(const char **error);

/**
 * @brief Frees a macro command instance.
 *
 * Releases memory and cleans up internal state.
 *
 * @param command Pointer to the macro command pointer to free. After the call, *command is set to NULL.
 */
void puremvc_macro_command_free(struct IMacroCommand **command);
