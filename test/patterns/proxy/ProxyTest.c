#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "puremvc/puremvc.h"
#include "ProxyTest.h"

int main(void) {
    testConstructor();
    testNameAccessors();
    testDataAccessors();
    testDataAccessors2();
    testDataReassign();
    return 0;
}

/**
 * Test Constructor
 */
void testConstructor() {
    const char *error = NULL;
    struct IProxy* proxy = puremvc_proxy_new(NULL, NULL, &error);
    if (proxy == NULL) fprintf(stderr, "%s\n", error);

    // test assertions
    assert(proxy != NULL);
    assert(proxy->notifier != NULL);
    assert(strcmp(proxy->getName(proxy), PROXY_NAME) == 0);

    puremvc_proxy_free(&proxy);
    assert(proxy == NULL);
}

/**
 * Tests getting the name using Proxy class accessor method. Setting can only be done in constructor.
 */
void testNameAccessors() {
    const char *error = NULL;
    struct IProxy* proxy = puremvc_proxy_new("TestProxy", NULL, &error);
    assert(proxy != NULL);

    // test assertions
    assert(strcmp(proxy->getName(proxy), "TestProxy") == 0);
    puremvc_proxy_free(&proxy);
    assert(proxy == NULL);

    struct IProxy* proxy2 = puremvc_proxy_new(NULL, NULL, &error);
    assert(strcmp(proxy2->getName(proxy2), PROXY_NAME) == 0);
    puremvc_proxy_free(&proxy2);
    assert(proxy2 == NULL);
}

/**
 * Tests setting and getting the item using Proxy class accessor methods.
 */
void testDataAccessors() {
    const char **colors = malloc(sizeof(char*) * 4);
    memset(colors, 0, sizeof(char*) * 4);
    for(const char **data = (const char *[]) {"red", "green", "blue", NULL}, **cursor = colors; *data; data++, cursor++) {
        *cursor = strdup(*data);
    }

    const char *error = NULL;
    struct IProxy *proxy = puremvc_proxy_new("colors", colors, &error);

    const char **data = proxy->getData(proxy);

    // test assertions
    assert(strcmp(*data, "red") == 0);
    assert(strcmp(*(data + 1), "green") == 0);
    assert(strcmp(*(data + 2), "blue") == 0);
    assert(*(data + 3) == NULL);

    puremvc_proxy_free(&proxy);
    data = NULL; // avoid dangling pointer

    assert(proxy == NULL);
}

void testDataAccessors2() {
    const char *str = "{red, green, blue}";

    // Create a new Proxy and use accessors to set the item
    const char *error = NULL;
    struct IProxy* proxy = puremvc_proxy_new("colors", strcpy(malloc(strlen(str) + 1), str), &error); // owner: takes ownership of data

    const char *data = proxy->getData(proxy); // borrower: non-owning access

    // test assertions
    assert(strcmp(data, str) == 0);

    puremvc_proxy_free(&proxy); // owner: releases proxy and its data
    data = NULL; // avoid dangling pointer

    assert(proxy == NULL);
}

void testDataReassign() {
    const char **colors = malloc(sizeof(char*) * 4);
    memset(colors, 0, sizeof(char*) * 4);
    for(const char **data = (const char *[]) {"red", "green", "blue", NULL}, **cursor = colors; *data; data++, cursor++) {
        *cursor = strdup(*data);
    }

    const char *error = NULL;
    struct IProxy *proxy = puremvc_proxy_new("colors", colors, &error);

    // Re-assign the same data to ensure the proxy does not free it
    proxy->setData(proxy, colors);

    const char **data = proxy->getData(proxy);

    assert(strcmp(*data, "red") == 0);
    assert(strcmp(*(data + 1), "green") == 0);
    assert(strcmp(*(data + 2), "blue") == 0);
    assert(*(data + 3) == NULL);

    puremvc_proxy_free(&proxy);
    data = NULL; // avoid dangling pointer

    assert(proxy == NULL);
}
