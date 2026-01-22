/**
 * @file IController.h
 * @ingroup PureMVC
 *
 * @author Saad Shams <saad.shams@puremvc.org>
 * @copyright BSD 3-Clause License
 */
#pragma once

#include <stdbool.h>

#include "ICommand.h"

/**
 * @struct IController
 * @ingroup PureMVC
 * @brief IController Interface
 *
 * The Controller maps notifications to command factories and
 * is responsible for executing commands in response to notifications.
 */
struct IController {
    /** @brief Initializes the controller.
     * @param self Pointer to the Controller instance.
     * @param error Out-param for a static error string on failure (NULL on success).
     */
    void (*initializeController)(struct IController *self, const char **error);

    /** @brief Registers a command factory for a notification name.
     * @param self Pointer to the Controller instance.
     * @param notificationName Pointer to the notification triggering this command.
     * @param factory Factory function that creates a command instance.
     * @param error Out-param for a static error string on failure (NULL on success).
     */
    void (*registerCommand)(const struct IController *self, const char *notificationName, struct ICommand *(*factory)(const char **error), const char **error);

    /** @brief Executes a command for the given notification.
     * @param self Pointer to the Controller instance.
     * @param notification Pointer to the notification triggering this command.
     * @param error Out-param for a static error string on failure (NULL on success).
     */
    void (*executeCommand)(const struct IController *self, struct INotification *notification, const char **error);

    /** @brief Checks if a command is registered for a notification.
     * @param self Pointer to the Controller instance.
     * @param notificationName Pointer to the notification triggering this command.
     */
    bool (*hasCommand)(const struct IController *self, const char *notificationName);

    /** @brief Removes a command mapping for a notification.
     * @param self Pointer to the Controller instance.
     * @param notificationName Pointer to the notification triggering this command.
     */
    void (*removeCommand)(const struct IController *self, const char *notificationName);
};

/**
 * @brief Creates a new controller instance for a multiton key.
 *
 * @param key Multiton key.
 * @param error Out-param for a static error string on failure (NULL on success).
 * @return Pointer to the initialized IController, or NULL on failure.
 */
struct IController *puremvc_controller_new(const char *key, const char **error);

/**
 * @brief Frees a controller instance.
 *
 * @param controller Pointer to the controller pointer to free. After the call, *controller is set to NULL.
 */
void puremvc_controller_free(struct IController **controller);

/**
 * @brief Retrieves or creates a controller instance for a multiton key.
 *
 * @param key Multiton key.
 * @param factory Factory function used to create the controller if it does not exist.
 * @param error Out-param for a static error string on failure (NULL on success).
 * @return Pointer to the IController instance, or NULL on failure.
 */
struct IController *puremvc_controller_getInstance(const char *key, struct IController *(*factory)(const char *key, const char **error), const char **error);

/**
 * @brief Removes the controller instance for a multiton key.
 *
 * @param key Multiton key.
 */
void puremvc_controller_removeController(const char *key);
