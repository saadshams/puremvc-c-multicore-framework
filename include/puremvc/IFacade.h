/**
* @file IFacade.h
* @ingroup PureMVC
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
    /** @brief Initializes the facade and its internal components. */
    void (*initializeFacade)(struct IFacade *self);

    /** @brief Initializes the controller. */
    void (*initializeController)(struct IFacade *self);

    /** @brief Initializes the model. */
    void (*initializeModel)(struct IFacade *self);

    /** @brief Initializes the view. */
    void (*initializeView)(struct IFacade *self);

    /** @brief Registers a command factory for a notification name. */
    void (*registerCommand)(const struct IFacade *self, const char *notificationName, struct ICommand *(*factory)());

    /** @brief Checks if a command is registered for a notification. */
    bool (*hasCommand)(const struct IFacade *self, const char *notificationName);

    /** @brief Removes a command mapping for a notification. */
    void (*removeCommand)(const struct IFacade *self, const char *notificationName);

    /** @brief Registers a proxy with the facade. */
    void (*registerProxy)(const struct IFacade *self, struct IProxy *proxy);

    /** @brief Retrieves a proxy by name. */
    struct IProxy *(*retrieveProxy)(const struct IFacade *self, const char *proxyName);

    /** @brief Checks if a proxy is registered. */
    bool (*hasProxy)(const struct IFacade *self, const char *proxyName);

    /** @brief Removes and returns a proxy by name. */
    struct IProxy *(*removeProxy)(const struct IFacade *self, const char *proxyName);

    /** @brief Registers a mediator with the facade. */
    void (*registerMediator)(const struct IFacade *self, struct IMediator *mediator);

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
 * @return Pointer to the IFacade instance.
 */
struct IFacade *puremvc_facade_getInstance(const char *key, struct IFacade *(*factory)(const char *key, const char **error));

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
