#include "ViewTest.h"
#include "interfaces/View.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "ViewTestMediator.h"
#include "ViewTestMediator2.h"
#include "ViewTestMediator3.h"
#include "ViewTestMediator4.h"
#include "ViewTestMediator5.h"
#include "ViewTestMediator6.h"

typedef struct {} ViewComponent;

typedef struct {
    int value;
} ViewTestVar;

ViewTestVar *viewTestVar1;
ViewTestVar *viewTestVar2;

void viewTestMethod(void *context, Notification *notification) {
    viewTestVar1 = notification->getBody(notification);
}

void viewTestMethod2(void *context, Notification *notification) {
    viewTestVar2 = notification->getBody(notification);
}

int main() {
    testGetInstance();
    testRegisterAndNotifyObserver();
    testRegisterAndRetrieveMediator();
    testHasMediator();
    testRegisterAndRemoveMediator();
    testOnRegisterAndOnRemove();
    testSuccessiveRegisterAndRemoveMediator();
    testRemoveMediatorAndSubsequentNotify();
    testRemoveOneOfTwoMediatorsAndSubsequentNotify();
    testMediatorReregistration();
    testModifyObserverListDuringNotification();
    puts("ViewTest: Success");
    return 0;
}

void testGetInstance() {
    View *view = getViewInstance("ViewTestKey1", NewView);
    assert(view != NULL);
    assert(view == getViewInstance("ViewTestKey1", NewView));
}

void testRegisterAndNotifyObserver() {
    View *view = getViewInstance("ViewTestKey2", NewView);

    Observer *observer1 = NewObserver(viewTestMethod, NULL);
    view->registerObserver(view, "ViewTestNote1", observer1);

    Notification *note1 = NewNotification("ViewTestNote1", &(ViewTestVar) {10}, NULL);
    view->notifyObservers(view, note1);
    assert(viewTestVar1->value == 10);

    Observer *observer2 = NewObserver(viewTestMethod2, NULL);
    view->registerObserver(view, "ViewTestNote2", observer2);
    Notification *note2 = NewNotification("ViewTestNote2", &(ViewTestVar) {20}, NULL);
    view->notifyObservers(view, note2);
    assert(viewTestVar2->value == 20);

    // remove second observer
    viewTestVar2->value = 0;
    view->removeObserver(view, "ViewTestNote2", viewTestMethod2);
    view->notifyObservers(view, note2);
    assert(viewTestVar2->value == 0);

    // remove first observer
    viewTestVar1->value = 0;
    view->removeObserver(view, "ViewTestNote1", viewTestMethod);
    view->notifyObservers(view, note1);
    assert(viewTestVar1->value == 0);

    DeleteNotification(note1);
    DeleteNotification(note2);
}

void testRegisterAndRetrieveMediator() {
    View *view = getViewInstance("ViewTestKey3", NewView);

    ViewComponent viewComponent = {};
    view->registerMediator(view, NewMediator("testing", &viewComponent));

    Mediator *mediator = view->retrieveMediator(view, "testing");

    assert(mediator != NULL);
    assert(mediator->getViewComponent(mediator) == &viewComponent);
    assert(strcmp(mediator->getMediatorName(mediator), "testing") == 0);

    // clean up
    view->removeMediator(view, "testing");
    DeleteMediator(mediator);
}

void testHasMediator() {
    View *view = getViewInstance("ViewTestKey4", NewView);

    ViewComponent viewComponent = {};
    Mediator *mediator = NewMediator("hasMediatorTest", &viewComponent);
    view->registerMediator(view, mediator);

    assert(view->hasMediator(view, "hasMediatorTest") == true);

    view->removeMediator(view, "hasMediatorTest");
    assert(view->hasMediator(view, "hasMediatorTest") == false);
    DeleteMediator(mediator);
}

void testOnRegisterAndOnRemove() {
    View *view = getViewInstance("ViewTestKey5", NewView);
    ViewTest viewTest = {"", false, false, 0};

    Mediator *mediator = (Mediator *)NewTestMediator4(&viewTest);
    view->registerMediator(view, mediator);
    assert(viewTest.onRegisterCalled);

    view->removeMediator(view, ViewTestMediator4_NAME);
    assert(view->retrieveMediator(view, ViewTestMediator4_NAME) == NULL);
    assert(viewTest.onRemoveCalled);

    // clean up
    DeleteMediator(mediator);
}

void testRegisterAndRemoveMediator() {
    View *view = getViewInstance("ViewTestKey6", NewView);

    ViewComponent viewComponent = {};
    Mediator *mediator = NewMediator("testing", &viewComponent);
    view->registerMediator(view, mediator);

    Mediator *removedMediator = view->removeMediator(view, "testing");

    assert(strcmp(removedMediator->getMediatorName(removedMediator), "testing") == 0);
    assert(view->retrieveMediator(view, "testing") == NULL);

    DeleteMediator(mediator);
}

void testSuccessiveRegisterAndRemoveMediator() {
    View *view = getViewInstance("ViewTestKey7", NewView);

    ViewComponent viewComponent = {};
    Mediator *mediator = (Mediator *) NewViewTestMediator(&viewComponent);

    view->registerMediator(view, mediator);

    assert(view->retrieveMediator(view, ViewTestMediator_NAME) == mediator);

    view->removeMediator(view, ViewTestMediator_NAME);

    assert(view->removeMediator(view, ViewTestMediator_NAME) == NULL);

    assert(view->removeMediator(view, ViewTestMediator_NAME) == NULL);

    Mediator *mediator2 = NewMediator(ViewTestMediator_NAME, &viewComponent);
    view->registerMediator(view, mediator2);

    assert(view->retrieveMediator(view, ViewTestMediator_NAME) == mediator2);

    view->removeMediator(view, ViewTestMediator_NAME);

    assert(view->retrieveMediator(view, ViewTestMediator_NAME) == NULL);

    DeleteMediator(mediator);
    DeleteMediator(mediator2);
}

void testRemoveMediatorAndSubsequentNotify() {
    View *view = getViewInstance("ViewTestKey8", NewView);

    ViewTest viewTest = {};
    Mediator *mediator = (Mediator *) ViewTestMediator2New(&viewTest);
    view->registerMediator(view, mediator);

    Notification *notification1 = NewNotification(NOTE1, NULL, NULL);
    view->notifyObservers(view, notification1);
    assert(strcmp(viewTest.lastNotification, NOTE1) == 0);
    free(viewTest.lastNotification);

    Notification *notification2 = NewNotification(NOTE2, NULL, NULL);
    view->notifyObservers(view, notification2);
    assert(strcmp(viewTest.lastNotification, NOTE2) == 0);
    free(viewTest.lastNotification);

    view->removeMediator(view, ViewTestMediator2_NAME);

    assert(view->retrieveMediator(view, ViewTestMediator2_NAME) == NULL);

    viewTest.lastNotification = "";

    view->notifyObservers(view, notification1);
    assert(strcmp(viewTest.lastNotification, NOTE1) != 0);

    view->notifyObservers(view, notification2);
    assert(strcmp(viewTest.lastNotification, NOTE2) != 0);

    DeleteNotification(notification1);
    DeleteNotification(notification2);
    DeleteMediator(mediator);
}

void testRemoveOneOfTwoMediatorsAndSubsequentNotify() {
    View *view = getViewInstance("ViewTestKey9", NewView);
    ViewTest viewTest = {};

    Mediator *mediator2 = (Mediator *)ViewTestMediator2New(&viewTest);
    view->registerMediator(view, mediator2);

    Mediator *mediator3 = (Mediator *)NewViewTestMediator3(&viewTest);
    view->registerMediator(view, mediator3);

    Notification *notification1 = NewNotification(NOTE1, NULL, NULL);
    view->notifyObservers(view, notification1);
    assert(strcmp(viewTest.lastNotification, NOTE1) == 0);
    free(viewTest.lastNotification);

    Notification *notification2 = NewNotification(NOTE2, NULL, NULL);
    view->notifyObservers(view, notification2);
    assert(strcmp(viewTest.lastNotification, NOTE2) == 0);
    free(viewTest.lastNotification);

    Notification *notification3 = NewNotification(NOTE3, NULL, NULL);
    view->notifyObservers(view, notification3);
    assert(strcmp(viewTest.lastNotification, NOTE3) == 0);
    // free(viewTest.lastNotification);

    view->removeMediator(view, ViewTestMediator2_NAME);
    assert(view->retrieveMediator(view, ViewTestMediator2_NAME) == NULL);
    assert(view->retrieveMediator(view, ViewTestMediator3_NAME) != NULL);

    view->notifyObservers(view, notification1);
    assert(strcmp(viewTest.lastNotification, NOTE1) != 0);

    view->notifyObservers(view, notification2);
    assert(strcmp(viewTest.lastNotification, NOTE2) != 0);

    view->notifyObservers(view, notification3);
    assert(strcmp(viewTest.lastNotification, NOTE3) == 0);

    DeleteMediator(mediator2);
    DeleteMediator(mediator3);
    DeleteNotification(notification1);
    DeleteNotification(notification2);
    DeleteNotification(notification3);
}

void testMediatorReregistration() {
    View *view = getViewInstance("ViewTestKey10", NewView);
    ViewTest viewTest = {};

    Mediator *mediator = (Mediator *) NewViewTestMediator5(&viewTest);

    // try to register another instance of that mediator (uses the same NAME constant).
    view->registerMediator(view, mediator);

    // test that the counter is only incremented once (mediator 5's response)
    viewTest.counter = 0;
    Notification *notification = NewNotification(NOTE5, NULL, NULL);
    view->notifyObservers(view, notification);
    assert(viewTest.counter == 1);

    // Remove the Mediator
    Mediator *removed = view->removeMediator(view, ViewTestMediator5_NAME);
    DeleteMediator(removed);

    // test that retrieving it now returns null
    assert(view->retrieveMediator(view, ViewTestMediator5_NAME) == NULL);

    // test that the counter is no longer incremented
    viewTest.counter = 0;
    view->notifyObservers(view, notification);
    assert(viewTest.counter == 0);

    DeleteNotification(notification);
}

void testModifyObserverListDuringNotification() {
    View *view = getViewInstance("ViewTestKey11", NewView);

    ViewTest viewTest = {0};

    view->registerMediator(view, (Mediator *) NewViewTestMediator6("ViewTestMediator6/1", &viewTest));
    view->registerMediator(view, (Mediator *) NewViewTestMediator6("ViewTestMediator6/2", &viewTest));
    view->registerMediator(view, (Mediator *) NewViewTestMediator6("ViewTestMediator6/3", &viewTest));
    view->registerMediator(view, (Mediator *) NewViewTestMediator6("ViewTestMediator6/4", &viewTest));
    view->registerMediator(view, (Mediator *) NewViewTestMediator6("ViewTestMediator6/5", &viewTest));
    view->registerMediator(view, (Mediator *) NewViewTestMediator6("ViewTestMediator6/6", &viewTest));
    view->registerMediator(view, (Mediator *) NewViewTestMediator6("ViewTestMediator6/7", &viewTest));
    view->registerMediator(view, (Mediator *) NewViewTestMediator6("ViewTestMediator6/8", &viewTest));

    Notification *notification = NewNotification(NOTE6, NULL, NULL);
    view->notifyObservers(view, notification);

    assert(viewTest.counter == 8);

    viewTest.counter = 0;
    view->notifyObservers(view, notification);
    assert(viewTest.counter == 0);

    DeleteNotification(notification);
}