#include "interfaces/Proxy.h"
#include "interfaces/Notifier.h"
#include "ProxyTest.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

/**
 * Test the PureMVC Proxy class.
 *
 * @see Proxy
 */
int main(int argc, char *argv[]) {
    testNameAccessor();
    testDataAccessors();
    testConstructor();
    puts("ProxyTest: Success");
    return 0;
}

/**
 * Tests getting the name using Proxy class accessor method. Setting can only be done in constructor.
 */
void testNameAccessor() {
    Proxy *proxy = $Proxy.new("TestProxy", NULL);
    proxy->notifier->initializeNotifier(proxy->notifier, "test");

    // test assertions
    assert(proxy != NULL);
    assert(proxy->notifier != NULL);
    assert(strcmp(proxy->getProxyName(proxy), "TestProxy") == 0);
    $Proxy.delete(proxy);
}

/**
 * Tests setting and getting the data using Proxy class accessor methods.
 */
void testDataAccessors() {
    // Create a new Proxy and use accessors to set the data
    Proxy *proxy = $Proxy.new("colors", NULL);
    proxy->notifier->initializeNotifier(proxy->notifier, "test");
    assert(proxy != NULL);
    proxy->setData(proxy, (const char *[]) {"red", "green", "blue", NULL});

    const char **data = proxy->getData(proxy);

    // test assertions
    assert(strcmp(*data, "red") == 0);
    assert(strcmp(*(data + 1), "green") == 0);
    assert(strcmp(*(data + 2), "blue") == 0);
    assert(*(data + 3) == NULL);

    $Proxy.delete(proxy);
}

/**
 * Tests setting the name and body using the Notification class Constructor.
 */
void testConstructor() {
    // Create a new Proxy using the Constructor to set the name and data
    Proxy *proxy = $Proxy.new("colors", (char *[]) {"red", "green", "blue", NULL});
    proxy->notifier->initializeNotifier(proxy->notifier, "test");
    const char **data = proxy->getData(proxy);

    // test assertions
    assert(proxy != NULL);
    assert(strcmp(proxy->getProxyName(proxy), "colors") == 0);

    assert(strcmp(*data, "red") == 0);
    assert(strcmp(*(data + 1), "green") == 0);
    assert(strcmp(*(data + 2), "blue") == 0);
    assert(*(data + 3) == NULL);

    $Proxy.delete(proxy);
}
