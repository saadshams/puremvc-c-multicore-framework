#include "MediatorTest.h"
#include "interfaces/Mediator.h"
#include "interfaces/Notifier.h"
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int main() {
    testNameAccessor();
    testViewAccessor();
    testListNotificationInterests();
    puts("MediatorTest: Success");
    return 0;
}

void testNameAccessor() {
    Mediator *mediator = NewMediator(NULL, NULL);
    mediator->notifier->initializeNotifier(mediator->notifier, "test");
    assert(strcmp(mediator->getMediatorName(mediator), MEDIATOR_NAME) == 0);
    DeleteMediator(mediator);
}

void testViewAccessor() {
    struct Object {} object;
    Mediator *mediator = NewMediator(MEDIATOR_NAME, &object);
    mediator->notifier->initializeNotifier(mediator->notifier, "test");
    assert(mediator->getViewComponent(mediator) == &object);
    DeleteMediator(mediator);
}

void testListNotificationInterests() {
    struct Object {} object;
    Mediator *mediator = NewMediator(MEDIATOR_NAME, &object);
    mediator->notifier->initializeNotifier(mediator->notifier, "test");
    const char * const *interests = mediator->listNotificationInterests(mediator);

    int i = 0;
    for (const char * const *cursor = interests; *cursor != NULL; cursor++) {
        i++;
    }
    assert(i == 0);
    DeleteMediator(mediator);
}