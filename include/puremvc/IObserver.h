/**
 * @file IObserver.h
 * @ingroup PureMVC
 *
 * @author Saad Shams <saad.shams@puremvc.org>
 * @copyright BSD 3-Clause License
 */
#pragma once

#include <stdbool.h>

#include "INotification.h"

/**
 * @struct IObserver
 * @brief Represents an observer in the PureMVC framework.
 *
 * This structure holds a notification callback and a context.
 * It provides methods to get/set the context, get/set the notify function,
 * notify the observer, and compare the notify context.
 */
struct IObserver {
    /**
     * @brief Gets the observer's context.
     *
     * @param observer Pointer to this observer instance.
     * @return Pointer to the context associated with the observer.
     */
    void *(*getContext)(const struct IObserver *observer);

    /**
     * @brief Sets the observer's context.
     *
     * @param observer Pointer to this observer instance.
     * @param notifyContext Pointer to the new context.
     */
    void (*setContext)(struct IObserver *observer, void *notifyContext);

    /**
     * @brief Gets the notify function.
     *
     * @param self Pointer to this observer instance.
     * @return Function pointer to the notification callback.
     */
    void (*(*getNotify)(const struct IObserver *self))(const void *context, struct INotification *notification);

    /**
     * @brief Sets the notify function.
     *
     * @param observer Pointer to this observer instance.
     * @param notify Function pointer to set as the notification callback.
     */
    void (*setNotify)(struct IObserver *observer, void (*notify)(const void *context, struct INotification *notification));

    /**
     * @brief Notifies the observer.
     *
     * Calls the notification callback with the provided notification.
     *
     * @param observer Pointer to this observer instance.
     * @param notification Pointer to the notification to send.
     */
    void (*notifyObserver)(const struct IObserver *observer, struct INotification *notification);

    /**
     * @brief Compares the observer's context with a given context.
     *
     * @param observer Pointer to this observer instance.
     * @param context Pointer to the context to compare.
     * @return true if the contexts match, false otherwise.
     */
    bool (*compareNotifyContext)(const struct IObserver *observer, const void *context);
};

/**
 * @brief Creates a new observer.
 *
 * Allocates memory and initializes the observer with the given notify function and context.
 *
 * @param notify Pointer to the notification callback function.
 * @param context Pointer to the context associated with the observer.
 * @param error Out-param for static error string on failure (NULL on success).
 * @return Pointer to the initialized IObserver, or NULL on failure.
 */
struct IObserver *puremvc_observer_new(const void (*notify)(const void *context, struct INotification *notification), const void *context, const char **error);

/**
 * @brief Frees an observer instance.
 *
 * Releases memory and cleans up internal state.
 *
 * @param observer Pointer to the observer pointer to free. After the call, *observer is set to NULL.
 */
void puremvc_observer_free(struct IObserver **observer);
