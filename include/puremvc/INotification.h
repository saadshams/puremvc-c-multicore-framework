/**
 * @file INotification.h
 * @ingroup PureMVC
 * @brief INotification Interface
 *
 * @author Saad Shams <saad.shams@puremvc.org>
 * @copyright BSD 3-Clause License
 */
#pragma once

/**
 * @struct INotification
 * @brief Represents a notification in the PureMVC framework.
 *
 * This structure encapsulates the information for a notification,
 * including its name, optional body, and optional type.
 * It provides methods to access and modify these fields, and
 * to generate a string representation of the notification.
 */
struct INotification {
    /**
     * @brief Gets the name of the notification.
     *
     * @param self Pointer to this notification instance.
     * @return Static string representing the notification name.
     */
    const char *(*getName)(const struct INotification *self);

    /**
     * @brief Gets the body of the notification.
     *
     * @param self Pointer to this notification instance.
     * @return Pointer to the notification body (can be NULL).
     */
    const void *(*getBody)(const struct INotification *self);

    /**
     * @brief Sets the body of the notification.
     *
     * @param self Pointer to this notification instance.
     * @param body Pointer to the data to attach to the notification.
     */
    void (*setBody)(struct INotification *self, void *body);

    /**
     * @brief Gets the type of the notification.
     *
     * @param self Pointer to this notification instance.
     * @return Static string representing the type (can be NULL).
     */
    const char *(*getType)(const struct INotification *self);

    /**
     * @brief Sets the type of the notification.
     *
     * @param self Pointer to this notification instance.
     * @param type Pointer to the type string to assign.
     */
    void (*setType)(struct INotification *self, const char *type);

    /**
     * @brief Returns a string representation of the notification.
     *
     * @param self Pointer to this notification instance.
     * @return Static string describing the notification.
     */
    const char *(*toString)(const struct INotification *self);
};

/**
 * @brief Creates a new notification instance.
 *
 * Allocates memory and initializes the notification with the given name, body, and type.
 *
 * @param name Name of the notification.
 * @param body Optional pointer to the notification body.
 * @param type Optional type string.
 * @param error Out-param for a static error string on failure (NULL on success).
 * @return Pointer to the initialized INotification, or NULL on failure.
 */
struct INotification *puremvc_notification_new(const char *name, void *body, const char *type, const char **error);

/**
 * @brief Frees a notification instance.
 *
 * Releases memory and cleans up internal state.
 *
 * @param notification Pointer to the notification pointer to free. After the call, *notification is set to NULL.
 */
void puremvc_notification_free(struct INotification **notification);
