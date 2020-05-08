#include "MediatorTest.h"
#include "interfaces/Mediator.h"
#include <assert.h>
#include <string.h>
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
    assert(strcmp(mediator->getMediatorName(mediator), MEDIATOR_NAME) == 0);
    DeleteMediator(mediator);
}

void testViewAccessor() {
    struct Object {} object;
    Mediator *mediator = NewMediator(MEDIATOR_NAME, &object);
    assert(mediator->getViewComponent(mediator) == &object);
    DeleteMediator(mediator);
}

void testListNotificationInterests() {
    struct Object {} object;
    Mediator *mediator = NewMediator(MEDIATOR_NAME, &object);
    char **interests = mediator->listNotificationInterests(mediator);

    int i = 0;
    while (*interests) {
        i++;
        interests++;
    }
    assert(i == 0);
}