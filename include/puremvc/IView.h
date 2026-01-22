/**
 * @file IView.h
 * @ingroup PureMVC
 *
 * @author Saad Shams <saad.shams@puremvc.org>
 * @copyright BSD 3-Clause License
 */
#pragma once

#include <stdbool.h>

#include "IMediator.h"
#include "INotification.h"
#include "IObserver.h"

/**
 * @struct IView
 * @brief Represents the View in the PureMVC framework.
 *
 * The View manages observers and mediators and is responsible
 * for delivering notifications to interested parties.
 */
struct IView {
    /** @brief Initializes the view. */
    void (*initializeView)(struct IView *self);

    /** @brief Registers an observer for a notification name. */
    void (*registerObserver)(const struct IView *self, const char *notificationName, const struct IObserver *observer);

    /** @brief Notifies all observers of a notification. */
    void (*notifyObservers)(const struct IView *self, const struct INotification *notification);

    /** @brief Removes an observer by notification name and context. */
    void (*removeObserver)(const struct IView *self, const char *notificationName, const void *notifyContext);

    /** @brief Registers a mediator with the view. */
    void (*registerMediator)(const struct IView *self, struct IMediator *mediator);

    /** @brief Retrieves a mediator by name. */
    struct IMediator *(*retrieveMediator)(const struct IView *self, const char *mediatorName);

    /** @brief Checks if a mediator is registered. */
    bool (*hasMediator)(const struct IView *self, const char *mediatorName);

    /** @brief Removes and returns a mediator by name. */
    struct IMediator *(*removeMediator)(const struct IView *self, const char *mediatorName);
};

/**
 * @brief Creates a new view instance for a multiton key.
 *
 * @param key Multiton key.
 * @param error Out-param for a static error string on failure (NULL on success).
 * @return Pointer to the initialized IView, or NULL on failure.
 */
struct IView *puremvc_view_new(const char *key, const char **error);

/**
 * @brief Frees a view instance.
 *
 * @param view Pointer to the view pointer to free. After the call, *view is set to NULL.
 */
void puremvc_view_free(struct IView **view);

/**
 * @brief Retrieves or creates a view instance for a multiton key.
 *
 * @param key Multiton key.
 * @param factory Factory function used to create the view if it does not exist.
 * @return Pointer to the IView instance.
 */
struct IView *puremvc_view_getInstance(const char *key, struct IView *(*factory)(const char *key, const char **error));

/**
 * @brief Removes the view instance for a multiton key.
 *
 * @param key Multiton key.
 */
void puremvc_view_removeView(const char *key);
