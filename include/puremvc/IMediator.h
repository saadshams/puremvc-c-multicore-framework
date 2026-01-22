#pragma once

#include "INotification.h"
#include "INotifier.h"

#define MEDIATOR_NAME "mediator"

/**
 * @struct IMediator
 * @brief Represents a mediator in the PureMVC framework.
 *
 * This structure holds references to the mediator's name,
 * its associated component, and provides hooks for handling
 * notifications and lifecycle events.
 */
struct IMediator {
    struct INotifier *notifier;

    const char *(*getName)(const struct IMediator *self);
    void (*setComponent)(struct IMediator *self, void *component);
    void *(*getComponent)(const struct IMediator *self);

    char **(*allocNotificationInterests)(const struct IMediator *self, const char **interests);
    void (*freeNotificationInterests)(const struct IMediator *self, char **interests);

    char **(*listNotificationInterests)(const struct IMediator *self);
    void (*handleNotification)(const struct IMediator *self, struct INotification *notification);

    void (*onRegister)(struct IMediator *self);
    void (*onRemove)(struct IMediator *self);
};

/**
 * @brief Creates a new mediator.
 *
 * Allocates memory and sets up internal state for a mediator instance.
 *
 * @param name The name of the mediator.
 * @param component Pointer to the associated component.
 * @param error Out-param for static error string on failure (NULL on success).
 * @return A pointer to the initialized mediator, or NULL on failure.
 */
struct IMediator *puremvc_mediator_new(const char *name, void *component, const char **error);

/**
 * @brief Frees a mediator instance.
 *
 * Releases memory and cleans up internal state for a mediator.
 *
 * @param mediator Pointer to the mediator pointer to free. After the call, *mediator is set to NULL.
 */
void puremvc_mediator_free(struct IMediator **mediator);
