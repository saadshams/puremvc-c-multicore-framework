/**
 * @file IProxy.h
 * @brief Implements core logic for data processing.
 *
 * This module provides functions to initialize, process,
 * and clean up resources used in data processing.
 *
 * @author Saad Shams
 * @copyright BSD 3-Clause License
 */
#pragma once

#include "INotifier.h"

#define PROXY_NAME "Proxy"

/**
 * @struct IProxy
 * @brief Represents the state of a data processor.
 *
 * This structure keeps track of memory allocation,
 * current processing index, and results.
 */
struct IProxy {
    struct INotifier *notifier;

    const char* (*getName)(const struct IProxy* self);
    void* (*getData)(const struct IProxy* self);
    void (*setData)(struct IProxy* self, void* data);

    void (*onRegister)(struct IProxy* self);
    void (*onRemove)(struct IProxy* self);
};

/**
 * @brief Initializes the data processor.
 *
 * Allocates memory and sets up internal state needed for data processing.
 *
 * @param name The number of elements to allocate.
 * @param data
 * @return A pointer to the initialized processor, or NULL on failure.
 */
struct IProxy* puremvc_proxy_new(const char* name, void* data);

/**
 * @brief Initializes the data processor.
 *
 * Allocates memory and sets up internal state needed for data processing.
 *
 * @param proxy
 * @return A pointer to the initialized processor, or NULL on failure.
 */
void puremvc_proxy_free(struct IProxy** proxy);
