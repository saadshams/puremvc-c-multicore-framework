#include "MediatorTest.h"
#include "interfaces/Mediator.h"
#include "interfaces/Notifier.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>

/**
 * Test the PureMVC Mediator class.
 *
 * @see org.puremvc.c.multicore.interfaces.Mediator Mediator
 * @see Mediator Mediator
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
    Mediator *mediator = NewMediator(NULL, NULL);
    mediator->notifier->initializeNotifier(mediator->notifier, "test");

    // test assertions
    assert(strcmp(mediator->getMediatorName(mediator), MEDIATOR_NAME) == 0);
    DeleteMediator(mediator);
}

/**
 * Tests getting the name using Mediator class accessor method.
 */
void testViewAccessor() {
    // Create a view object
    struct View {} view;
    Mediator *mediator = NewMediator(MEDIATOR_NAME, &view);
    mediator->notifier->initializeNotifier(mediator->notifier, "test");

    // test assertions
    assert(mediator->getViewComponent(mediator) == &view);
    DeleteMediator(mediator);
}

/**
 * Tests getting the notification list using Mediator class accessor method.
 */
void testListNotificationInterests() {
    // Create a view object
    struct View {} view;

    // Create a new Mediator and pass the view object
    Mediator *mediator = NewMediator(MEDIATOR_NAME, &view);
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
    DeleteMediator(mediator);
}