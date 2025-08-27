/**
 * @file Proxy.h
 * @brief Implements core logic for data processing.
 *
 * This module provides functions to initialize, process,
 * and clean up resources used in data processing.
 *
 * @author Saad Shams
 * @copyright BSD 3-Clause License
 */
#pragma once

#include "puremvc/IProxy.h"

struct Proxy {
    struct IProxy base;

    const char* name;
    void* data;
};
