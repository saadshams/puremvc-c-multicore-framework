#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "puremvc/puremvc.h"

/**
 * Test Constructor
 */
void testConstructor() {
    struct IMediator *mediator = puremvc_mediator_new(NULL, NULL);

    // test assertions
    assert(mediator != NULL);
    // assert(mediator->notifier != NULL);
    assert(strcmp(mediator->getName(mediator), MEDIATOR_NAME) == 0);
    puremvc_mediator_free(&mediator);
    assert(mediator == NULL);
}

/**
 * Tests getting the name using Mediator class accessor method.
 */
void testNameAccessor() {
    struct IMediator *mediator = puremvc_mediator_new("TestMediator", NULL);
    // mediator->notifier->initializeNotifier(mediator->notifier, "test");

    // test assertions
    assert(mediator != NULL);
    // assert(mediator->notifier != NULL);
    assert(strcmp(mediator->getName(mediator), "TestMediator") == 0);
    puremvc_mediator_free(&mediator);
    assert(mediator == NULL);

    struct IMediator *mediator2 = puremvc_mediator_new(NULL, NULL);
    puremvc_mediator_free(&mediator2);
    assert(mediator2 == NULL);

    struct IMediator *mediator3 = puremvc_mediator_new(NULL, NULL);
    puremvc_mediator_free(&mediator3);
    assert(mediator3 == NULL);
}

/**
 * Tests getting the viewComponent using Mediator class accessor method.
 */
void testViewAccessor() {
    // Create a view object
    struct Component {} component;
    struct IMediator *mediator = puremvc_mediator_new(MEDIATOR_NAME, &component);
    // mediator->notifier->initializeNotifier(mediator->notifier, "test");

    // test assertions
    assert(mediator->getComponent(mediator) == &component);
    mediator->setComponent(mediator, NULL);
    assert(mediator->getComponent(mediator) == NULL);
    puremvc_mediator_free(&mediator);
    assert(mediator == NULL);
}

int main() {
    testConstructor();
    testNameAccessor();
    testViewAccessor();
    return 0;
}
