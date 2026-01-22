/**
* @file ICommand.h
* @ingroup PureMVC
*
* @author Saad Shams <saad.shams@puremvc.org>
* @copyright BSD 3-Clause License
*/
#pragma once

#include "INotification.h"
#include "INotifier.h"

struct ICommand {
    struct INotifier *notifier;

    /**
     * @brief Executes the command.
     *
     * @param self Pointer to this command instance.
     * @param notification Pointer to the notification triggering this command.
     */
    void (*execute)(const struct ICommand *self, struct INotification *notification);
};

/**
 * @brief Creates a new command.
 *
 * Allocates memory and initializes the command structure.
 *
 * @param error Out-param for static error string on failure (NULL on success).
 * @return A pointer to the initialized ICommand, or NULL on failure.
 */
struct ICommand *puremvc_simple_command_new(const char **error);

/**
 * @brief Frees a command instance.
 *
 * Releases memory and cleans up internal state.
 *
 * @param command Pointer to the command pointer to free. After the call, *command is set to NULL.
 */
void puremvc_simple_command_free(struct ICommand **command);
