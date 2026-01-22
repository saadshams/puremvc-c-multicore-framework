/**
 * @file IModel.h
 * @ingroup PureMVC
 * @brief IModel Interface
 *
 * @author Saad Shams <saad.shams@puremvc.org>
 * @copyright BSD 3-Clause License
 */
#pragma once

#include <stdbool.h>

#include "IProxy.h"

/**
 * @struct IModel
 * @brief Interface for the PureMVC Model component.
 *
 * The Model is responsible for maintaining a collection of
 * Proxy instances. Each Proxy is registered and accessed
 * by a unique name.
 *
 * Implementations must ensure thread-safety if used in
 * concurrent environments.
 */
struct IModel {
    /**
     * @brief Initializes the Model instance.
     *
     * This function must be called before using the Model.
     * Typically, invoked internally during construction.
     *
     * @param self Pointer to the Model instance.
     * @param error Out-param for a static error string on failure (NULL on success).
     */
    void (*initializeModel)(struct IModel *self, const char **error);

    /**
     * @brief Registers a Proxy with the Model.
     *
     * If a Proxy with the same name already exists, the
     * behavior is implementation-defined (typically fatal).
     *
     * @param self  Pointer to the Model instance.
     * @param proxy Pointer to the Proxy to register.
     */
    void (*registerProxy)(const struct IModel *self, struct IProxy *proxy);

    /**
     * @brief Retrieves a registered Proxy by name.
     *
     * @param self      Pointer to the Model instance.
     * @param proxyName Name of the Proxy to retrieve.
     *
     * @return Pointer to the Proxy if found, or NULL if not found.
     */
    struct IProxy *(*retrieveProxy)(const struct IModel *self, const char *proxyName);

    /**
     * @brief Checks whether a Proxy is registered.
     *
     * @param self      Pointer to the Model instance.
     * @param proxyName Name of the Proxy.
     *
     * @return true if the Proxy exists, false otherwise.
     */
    bool (*hasProxy)(const struct IModel *self, const char *proxyName);

    /**
     * @brief Removes a Proxy from the Model.
     *
     * The removed Proxy is returned to the caller, who
     * becomes responsible for its lifecycle.
     *
     * @param self      Pointer to the Model instance.
     * @param proxyName Name of the Proxy to remove.
     *
     * @return Pointer to the removed Proxy, or NULL if not found.
     */
    struct IProxy *(*removeProxy)(const struct IModel *self, const char *proxyName);
};

/**
 * @brief Creates a new Model instance.
 *
 * This function allocates and initializes a new Model
 * associated with the specified key.
 *
 * @param key Unique identifier for the Model instance.
 * @param error Out-param for a static error string on failure (NULL on success).
 *
 * @return Pointer to a new Model instance, or NULL on failure.
 */
struct IModel *puremvc_model_new(const char *key, const char **error);

/**
 * @brief Destroys a Model instance.
 *
 * Frees all internal resources associated with the Model.
 * After this call, the Model pointer is set to NULL.
 *
 * @param model Address of the Model pointer to destroy.
 */
void puremvc_model_free(struct IModel **model);

/**
 * @brief Retrieves or creates a Model singleton instance.
 *
 * If a Model associated with the given key already exists,
 * it is returned. Otherwise, a new instance is created
 * using the provided factory function.
 *
 * @param key     Unique identifier for the Model instance.
 * @param factory Factory function used to create the Model if it does not exist.
 * @param error Out-param for a static error string on failure (NULL on success).
 *
 * @return Pointer to the Model instance.
 */
struct IModel *puremvc_model_getInstance(const char *key, struct IModel *(*factory)(const char *key, const char **error), const char **error);

/**
 * @brief Removes a Model instance associated with a key.
 *
 * This function removes the Model from the internal
 * instance map and releases its resources.
 *
 * @param key Unique identifier for the Model instance.
 */
void puremvc_model_removeModel(const char *key);
