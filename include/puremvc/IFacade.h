/**
 * @file IFacade.h
 * @ingroup PureMVC
 * @brief IFacade Interface
 *
 * @author Saad Shams <saad.shams@puremvc.org>
 * @copyright BSD 3-Clause License
 */
#pragma once

#include <stdbool.h>

#include "ICommand.h"
#include "IMediator.h"
#include "IProxy.h"
#include "INotification.h"

/**
 * @struct IFacade
 * @brief Represents the Facade in the PureMVC framework.
 *
 * The Facade provides a unified interface to the Model, View,
 * and Controller layers. It manages commands, proxies, mediators,
 * and notification dispatching for a given multiton key.
 */
struct IFacade {
    /** @brief Initializes the facade and its internal components.
     * @param self Pointer to the Facade instance.
     * @param error Out-param for a static error string on failure (NULL on success).
     */
    void (*initializeFacade)(struct IFacade *self, const char **error);

    /** @brief Initializes the controller.
     * @param self Pointer to the Facade instance.
     * @param error Out-param for a static error string on failure (NULL on success).
     */
    void (*initializeController)(struct IFacade *self, const char **error);

    /** @brief Initializes the model.
     * @param self Pointer to the Facade instance.
     * @param error Out-param for a static error string on failure (NULL on success).
     */
    void (*initializeModel)(struct IFacade *self, const char **error);

    /** @brief Initializes the view.
     * @param self Pointer to the Facade instance.
     * @param error Out-param for a static error string on failure (NULL on success).
     */
    void (*initializeView)(struct IFacade *self, const char **error);

    /** @brief Registers a command factory for a notification name.
     * @param self Pointer to the Facade instance.
     * @param notificationName Pointer to the notification triggering this command.
     * @param factory Factory function that creates a command instance.
     * @param error Out-param for a static error string on failure (NULL on success).
     */
    void (*registerCommand)(const struct IFacade *self, const char *notificationName, struct ICommand *(*factory)(), const char **error);

    /** @brief Checks if a command is registered for a notification.
     * @param self Pointer to the Facade instance.
     * @param notificationName Pointer to the notification triggering this command.
     */
    bool (*hasCommand)(const struct IFacade *self, const char *notificationName);

    /** @brief Removes a command mapping for a notification. */
    void (*removeCommand)(const struct IFacade *self, const char *notificationName);

    /** @brief Registers a proxy with the facade.
     * @param self Pointer to the Facade instance.
     * @param proxy Pointer to the Proxy to register.
     * @param error Out-param for a static error string on failure (NULL on success).
     */
    void (*registerProxy)(const struct IFacade *self, struct IProxy *proxy, const char **error);

    /** @brief Retrieves a proxy by name. */
    struct IProxy *(*retrieveProxy)(const struct IFacade *self, const char *proxyName);

    /** @brief Checks if a proxy is registered. */
    bool (*hasProxy)(const struct IFacade *self, const char *proxyName);

    /** @brief Removes and returns a proxy by name. */
    struct IProxy *(*removeProxy)(const struct IFacade *self, const char *proxyName);

    /** @brief Registers a mediator with the facade.
     * @param self Pointer to the Facade instance.
     * @param mediator Pointer to the Mediator to register.
     * @param error Out-param for a static error string on failure (NULL on success).
     */
    void (*registerMediator)(const struct IFacade *self, struct IMediator *mediator, const char **error);

    /** @brief Retrieves a mediator by name. */
    struct IMediator *(*retrieveMediator)(const struct IFacade *self, const char *mediatorName);

    /** @brief Checks if a mediator is registered. */
    bool (*hasMediator)(const struct IFacade *self, const char *mediatorName);

    /** @brief Removes and returns a mediator by name. */
    struct IMediator *(*removeMediator)(const struct IFacade *self, const char *mediatorName);

    /** @brief Notifies observers of a notification. */
    void (*notifyObservers)(const struct IFacade *self, const struct INotification *notification);

    /** @brief Sends a notification. */
    void (*sendNotification)(const struct IFacade *self, const char *notificationName, void *body, const char *type);
};

/**
 * @brief Creates a new facade instance for a multiton key.
 *
 * @param key Multiton key.
 * @param error Out-param for static error string on failure (NULL on success).
 * @return Pointer to the initialized IFacade, or NULL on failure.
 */
struct IFacade *puremvc_facade_new(const char *key, const char **error);

/**
 * @brief Frees a facade instance.
 *
 * @param facade Pointer to the facade pointer to free. After the call, *facade is set to NULL.
 */
void puremvc_facade_free(struct IFacade **facade);

/**
 * @brief Retrieves or creates a facade instance for a multiton key.
 *
 * @param key Multiton key.
 * @param factory Factory function used to create the facade if it does not exist.
 * @param error Out-param for a static error string on failure (NULL on success).
 * @return Pointer to the IFacade instance.
 */
struct IFacade *puremvc_facade_getInstance(const char *key, struct IFacade *(*factory)(const char *key, const char **error), const char **error);

/**
 * @brief Checks whether a facade core exists for a multiton key.
 *
 * @param key Multiton key.
 * @return true if a core exists, false otherwise.
 */
bool puremvc_facade_hasCore(const char *key);

/**
 * @brief Removes a facade instance for a multiton key.
 *
 * @param key Multiton key.
 */
void puremvc_facade_removeFacade(const char *key);
