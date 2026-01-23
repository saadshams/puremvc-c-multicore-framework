#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "puremvc/puremvc.h"
#include "MediatorTest.h"

int main() {
    testConstructor();
    testNameAccessor();
    testViewAccessor();
    return 0;
}

/**
 * Test Constructor
 */
void testConstructor() {
    const char *error = NULL;
    struct IMediator *mediator = puremvc_mediator_new(NULL, NULL, &error);

    // test assertions
    assert(mediator != NULL);
    assert(mediator->notifier != NULL);
    assert(strcmp(mediator->getName(mediator), MEDIATOR_NAME) == 0);
    puremvc_mediator_free(&mediator);
    assert(mediator == NULL);
}

/**
 * Tests getting the name using Mediator class accessor method.
 */
void testNameAccessor() {
    const char *error = NULL;
    struct IMediator *mediator = puremvc_mediator_new("TestMediator", NULL, &error);
    if (mediator == NULL) fprintf(stderr, "%s\n", error);

    // test assertions
    assert(mediator != NULL);
    assert(strcmp(mediator->getName(mediator), "TestMediator") == 0);
    puremvc_mediator_free(&mediator);
    assert(mediator == NULL);

    struct IMediator *mediator2 = puremvc_mediator_new(NULL, NULL, &error);
    puremvc_mediator_free(&mediator2);
    assert(mediator2 == NULL);

    struct IMediator *mediator3 = puremvc_mediator_new(NULL, NULL, &error);
    puremvc_mediator_free(&mediator3);
    assert(mediator3 == NULL);
}

/**
 * Tests getting the viewComponent using Mediator class accessor method.
 */
void testViewAccessor() {
    // Create a view object
    struct Component {int x;} component;
    const char *error = NULL;
    struct IMediator *mediator = puremvc_mediator_new(MEDIATOR_NAME, &component, &error);

    // test assertions
    assert(mediator->getComponent(mediator) == &component);
    mediator->setComponent(mediator, NULL);
    assert(mediator->getComponent(mediator) == NULL);
    puremvc_mediator_free(&mediator);
    assert(mediator == NULL);
}
