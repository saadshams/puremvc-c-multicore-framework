#include "MediatorTest.h"
#include "interfaces/Mediator.h"
#include "interfaces/Notifier.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>

/**
 * Test the PureMVC Mediator class.
 *
 * @see Mediator
 */
int main() {
    testNameAccessor();
    testViewAccessor();
    testListNotificationInterests();
    puts("MediatorTest: Success");
    return 0;
}

/**
 * Tests getting the name using Mediator class accessor method.
 */
void testNameAccessor() {
    // Create a new Mediator and use accessors to set the mediator name
    Mediator *mediator = $Mediator.new(NULL, NULL);
    mediator->notifier->initializeNotifier(mediator->notifier, "test");

    // test assertions
    assert(strcmp(mediator->getMediatorName(mediator), MEDIATOR_NAME) == 0);
    $Mediator.delete(mediator);
}

/**
 * Tests getting the name using Mediator class accessor method.
 */
void testViewAccessor() {
    // Create a view object
    struct View {} view;
    Mediator *mediator = $Mediator.new(MEDIATOR_NAME, &view);
    mediator->notifier->initializeNotifier(mediator->notifier, "test");

    // test assertions
    assert(mediator->getViewComponent(mediator) == &view);
    $Mediator.delete(mediator);
}

/**
 * Tests getting the notification list using Mediator class accessor method.
 */
void testListNotificationInterests() {
    // Create a view object
    struct View {} view;

    // Create a new Mediator and pass the view object
    Mediator *mediator = $Mediator.new(MEDIATOR_NAME, &view);
    mediator->notifier->initializeNotifier(mediator->notifier, "test");

    // retrieve notification list
    const char * const *interests = mediator->listNotificationInterests(mediator);

    // count notifications
    int i = 0;
    for (const char * const *cursor = interests; *cursor != NULL; cursor++) {
        i++;
    }

    // test assertions
    assert(i == 0);
    $Mediator.delete(mediator);
}