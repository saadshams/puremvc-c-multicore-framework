/**
 * @file INotifier.h
 * @ingroup PureMVC
 *
 * @author Saad Shams <saad.shams@puremvc.org>
 * @copyright BSD 3-Clause License
 */
#pragma once

struct IFacade; // Forward-declare IFacade to avoid including it and break reference cycle

/**
 * @struct INotifier
 * @brief Represents a notifier in the PureMVC framework.
 *
 * This structure provides an interface for sending notifications to the framework.
 * It holds function pointers for accessing the facade, retrieving the multiton key,
 * initializing the notifier, and sending notifications.
 */
struct INotifier {
    /**
     * @brief Gets the facade instance associated with this notifier.
     *
     * @param self Pointer to this notifier instance.
     * @param error Out-param for a static error string on failure (NULL on success).
     * @return Pointer to the IFacade instance.
     */
    struct IFacade *(*getFacade)(const struct INotifier *self, const char **error);

    /**
    * @brief Gets the multiton key for this notifier.
    *
    * @param self Pointer to this notifier instance.
    * @return Static string representing the key.
    */
    const char *(*getKey)(const struct INotifier *self);

    /**
     * @brief Initializes the notifier with a multiton key.
     *
     * Must be called before sending notifications.
     *
     * @param self Pointer to this notifier instance.
     * @param key Multiton key to associate with this notifier.
     */
    void (*initializeNotifier)(struct INotifier *self, const char *key);

    /**
     * @brief Sends a notification through the PureMVC framework.
     *
     * @param self Pointer to this notifier instance.
     * @param notificationName Name of the notification.
     * @param body Optional pointer to the notification body.
     * @param type Optional type string for the notification.
     * @param error Out-param for a static error string on failure (NULL on success).
     */
    void (*sendNotification)(const struct INotifier *self, const char *notificationName, void *body, const char *type, const char **error);
};

/**
 * @brief Creates a new notifier instance.
 *
 * Allocates memory and initializes the notifier structure.
 *
 * @param error Out-param for static error string on failure (NULL on success).
 * @return Pointer to the initialized INotifier, or NULL on failure.
 */
struct INotifier *puremvc_notifier_new(const char **error);

/**
 * @brief Frees a notifier instance.
 *
 * Releases memory and cleans up internal state.
 *
 * @param notifier Pointer to the notifier pointer to free. After the call, *notifier is set to NULL.
 */
void puremvc_notifier_free(struct INotifier **notifier);
