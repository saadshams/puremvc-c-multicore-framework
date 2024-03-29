#include "ViewTest.h"
#include "interfaces/View.h"
#include "interfaces/Facade.h"
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

typedef struct {
} ViewComponent;

/**
 * A test variable that proves the viewTestMethod was
 * invoked by the View.
 */
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

/**
 * Test the PureMVC View class.
 */
int main(int argc, char *argv[]) {
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
    testRemoveView();
    puts("ViewTest: Success");
    return 0;
}

/**
 * Tests the View Multiton Factory Method
 */
void testGetInstance() {
    // Test Factory Method
    View *view = $View.getInstance("ViewTestKey1", $View.new);

    // test assertions
    assert(view != NULL);
    assert(view == $View.getInstance("ViewTestKey1", $View.new));
    $View.removeView("ViewTestKey1");
    view = NULL;
}

/**
 * Tests registration and notification of Observers.
 *
 * <P>An Observer is created to callback the viewTestMethod of
 * this ViewTest instance. This Observer is registered with
 * the View to be notified of 'ViewTestEvent' events. Such
 * an event is created, and a value set on its payload. Then
 * the View is told to notify interested observers of this
 * Event.</P>
 *
 * <P>The View calls the Observer's notifyObserver method
 * which calls the viewTestMethod on this instance
 * of the ViewTest class. The viewTestMethod method will set
 * an instance variable to the value passed in on the Event
 * payload. We evaluate the instance variable to be sure
 * it is the same as that passed out as the payload of the
 * original 'ViewTestEvent'.</P>
 */
void testRegisterAndNotifyObserver() {
    // Get the Multiton View instance
    ViewComponent viewComponent = {};
    View *view = $View.getInstance("ViewTestKey2", $View.new);

    // Create observer, passing in notification method and context
    Observer *observer = $Observer.new(viewTestMethod, &viewComponent);

    // Register Observer's interest in a particular Notification with the View
    view->registerObserver(view, "ViewTestNote1", observer);

    // Create a ViewTestNote, setting
    // a body value, and tell the View to notify
    // Observers. Since the Observer is this class
    // and the notification method is viewTestMethod,
    // successful notification will result in our local
    // viewTestVar being set to the value we pass in
    // on the note body.
    Notification *note = $Notification.new("ViewTestNote1", &(ViewTestVar) {10}, NULL);
    view->notifyObservers(view, note);

    // test assertions
    assert(viewTestVar1->value == 10);

    // remove first observer
    viewTestVar1->value = 0;
    view->removeObserver(view, "ViewTestNote1", &viewComponent);
    view->notifyObservers(view, note);
    assert(viewTestVar1->value == 0);

    $Notification.delete(note);
    $View.removeView("ViewTestKey2");
    view = NULL;
}

/**
 * Tests registering and retrieving a mediator with
 * the View.
 */
void testRegisterAndRetrieveMediator() {
    // Get the Multiton View instance
    View *view = $View.getInstance("ViewTestKey3", $View.new);

    // Create and register the test mediator
    ViewComponent viewComponent = {};
    view->registerMediator(view, $Mediator.new("testing", &viewComponent));

    // Retrieve the component
    Mediator *mediator = view->retrieveMediator(view, "testing");

    // test assertions
    assert(mediator != NULL);
    assert(mediator->getViewComponent(mediator) == &viewComponent);
    assert(strcmp(mediator->getMediatorName(mediator), "testing") == 0);

    // clean up
    view->removeMediator(view, "testing");
    $Mediator.delete(mediator);
    $View.removeView("ViewTestKey3");
    view = NULL;
}

/**
 * Tests the hasMediator Method
 */
void testHasMediator() {
    // Get the Multiton View instance
    View *view = $View.getInstance("ViewTestKey4", $View.new);

    // Create and register the test mediator
    ViewComponent viewComponent = {};
    Mediator *mediator = $Mediator.new("hasMediatorTest", &viewComponent);
    view->registerMediator(view, mediator);

    // assert that the view.hasMediator method returns true
    // for that mediator name
    assert(view->hasMediator(view, "hasMediatorTest") == true);

    view->removeMediator(view, "hasMediatorTest");

    // assert that the view.hasMediator method returns false
    // for that mediator name
    assert(view->hasMediator(view, "hasMediatorTest") == false);

    $Mediator.delete(mediator);
    $View.removeView("ViewTestKey4");
    view = NULL;
}

/**
 * Tests registering and removing a mediator
 */
void testRegisterAndRemoveMediator() {
    // Get the Multiton View instance
    View *view = $View.getInstance("ViewTestKey6", $View.new);

    // Create and register the test mediator
    ViewComponent viewComponent = {};
    Mediator *mediator = $Mediator.new("testing", &viewComponent);
    view->registerMediator(view, mediator);

    // Remove the component
    Mediator *removedMediator = view->removeMediator(view, "testing");

    // assert that we have removed the appropriate mediator
    assert(strcmp(removedMediator->getMediatorName(removedMediator), "testing") == 0);

    // assert that the mediator is no longer retrievable
    assert(view->retrieveMediator(view, "testing") == NULL);

    $Mediator.delete(mediator);
    $View.removeView("ViewTestKey6");
    view = NULL;
}

/**
 * Tests that the View callse the onRegister and onRemove methods
 */
void testOnRegisterAndOnRemove() {
    // Get the Multiton View instance
    View *view = $View.getInstance("ViewTestKey5", $View.new);
    ViewTest viewTest = {"", false, false, 0};

    // Create and register the test mediator
    Mediator *mediator = (Mediator *) NewTestMediator4(&viewTest);
    view->registerMediator(view, mediator);

    // assert that onRegister was called, and the mediator responded by setting our boolean
    assert(viewTest.onRegisterCalled);

    // Remove the component
    view->removeMediator(view, ViewTestMediator4_NAME);

    // assert that the mediator is no longer retrievable
    assert(view->retrieveMediator(view, ViewTestMediator4_NAME) == NULL);

    // assert that onRemove was called, and the mediator responded by setting our boolean
    assert(viewTest.onRemoveCalled);

    // clean up
    $Mediator.delete(mediator);
    $View.removeView("ViewTestKey5");
    view = NULL;
}

/**
 * Tests successive register and remove of same mediator.
 */
void testSuccessiveRegisterAndRemoveMediator() {
    // Get the Multiton View instance
    View *view = $View.getInstance("ViewTestKey7", $View.new);

    // Create and register the test mediator,
    // but not so we have a reference to it
    ViewComponent viewComponent = {};
    Mediator *mediator = (Mediator *) NewViewTestMediator(&viewComponent);

    view->registerMediator(view, mediator);

    // test that we can retrieve it
    assert(view->retrieveMediator(view, ViewTestMediator_NAME) == mediator);

    // Remove the Mediator
    view->removeMediator(view, ViewTestMediator_NAME);

    // test that retrieving it now returns null
    assert(view->removeMediator(view, ViewTestMediator_NAME) == NULL);

    // test that removing the mediator again once its gone doesn't cause crash
    assert(view->removeMediator(view, ViewTestMediator_NAME) == NULL);

    // Create and register another instance of the test mediator,
    Mediator *mediator2 = $Mediator.new(ViewTestMediator_NAME, &viewComponent);
    view->registerMediator(view, mediator2);

    assert(view->retrieveMediator(view, ViewTestMediator_NAME) == mediator2);

    // Remove the Mediator
    view->removeMediator(view, ViewTestMediator_NAME);

    // test that retrieving it now returns null
    assert(view->retrieveMediator(view, ViewTestMediator_NAME) == NULL);

    $Mediator.delete(mediator);
    $Mediator.delete(mediator2);
    $View.removeView("ViewTestKey7");
    view = NULL;
}

/**
 * Tests registering a Mediator for 2 different notifications, removing the
 * Mediator from the View, and seeing that neither notification causes the
 * Mediator to be notified. Added for the fix deployed in version 1.7
 */
void testRemoveMediatorAndSubsequentNotify() {
    // Get the Multiton View instance
    View *view = $View.getInstance("ViewTestKey8", $View.new);

    // Create and register the test mediator to be removed.
    ViewTest viewTest = {};
    Mediator *mediator = (Mediator *) NewViewTestMediator2(&viewTest);
    view->registerMediator(view, mediator);

    // test that notifications work
    Notification *notification1 = $Notification.new(NOTE1, NULL, NULL);
    view->notifyObservers(view, notification1);
    assert(strcmp(viewTest.lastNotification, NOTE1) == 0);

    Notification *notification2 = $Notification.new(NOTE2, NULL, NULL);
    view->notifyObservers(view, notification2);
    assert(strcmp(viewTest.lastNotification, NOTE2) == 0);

    view->removeMediator(view, ViewTestMediator2_NAME);

    assert(view->retrieveMediator(view, ViewTestMediator2_NAME) == NULL);

    viewTest.lastNotification = "";

    view->notifyObservers(view, notification1);
    assert(strcmp(viewTest.lastNotification, NOTE1) != 0);

    view->notifyObservers(view, notification2);
    assert(strcmp(viewTest.lastNotification, NOTE2) != 0);

    $Notification.delete(notification1);
    $Notification.delete(notification2);
    $Mediator.delete(mediator);
    $View.removeView("ViewTestKey8");
    view = NULL;
}

/**
 * Tests registering one of two registered Mediators and seeing
 * that the remaining one still responds.
 */
void testRemoveOneOfTwoMediatorsAndSubsequentNotify() {
    // Get the Multiton View instance
    View *view = $View.getInstance("ViewTestKey9", $View.new);
    ViewTest viewTest = {};

    // Create and register that responds to notifications 1 and 2
    Mediator *mediator2 = (Mediator *) NewViewTestMediator2(&viewTest);
    view->registerMediator(view, mediator2);

    // Create and register that responds to notification 3
    Mediator *mediator3 = (Mediator *) NewViewTestMediator3(&viewTest);
    view->registerMediator(view, mediator3);

    // test that all notifications work
    Notification *notification1 = $Notification.new(NOTE1, NULL, NULL);
    view->notifyObservers(view, notification1);
    assert(strcmp(viewTest.lastNotification, NOTE1) == 0);

    Notification *notification2 = $Notification.new(NOTE2, NULL, NULL);
    view->notifyObservers(view, notification2);
    assert(strcmp(viewTest.lastNotification, NOTE2) == 0);

    Notification *notification3 = $Notification.new(NOTE3, NULL, NULL);
    view->notifyObservers(view, notification3);
    assert(strcmp(viewTest.lastNotification, NOTE3) == 0);

    // Remove the Mediator that responds to 1 and 2
    view->removeMediator(view, ViewTestMediator2_NAME);

    // test that retrieving it now returns null
    assert(view->retrieveMediator(view, ViewTestMediator2_NAME) == NULL);

    // test that notifications no longer work
    // for notifications 1 and 2, but still work for 3
    viewTest.lastNotification = "";

    view->notifyObservers(view, notification1);
    assert(strcmp(viewTest.lastNotification, NOTE1) != 0);

    view->notifyObservers(view, notification2);
    assert(strcmp(viewTest.lastNotification, NOTE2) != 0);

    view->notifyObservers(view, notification3);
    assert(strcmp(viewTest.lastNotification, NOTE3) == 0);

    view->removeMediator(view, ViewTestMediator3_NAME);

    $Mediator.delete(mediator2);
    $Mediator.delete(mediator3);

    $Notification.delete(notification1);
    $Notification.delete(notification2);
    $Notification.delete(notification3);
    $View.removeView("ViewTestKey9");
    view = NULL;
}

/**
 * Tests registering the same mediator twice.
 * A subsequent notification should only illicit
 * one response. Also, since reregistration
 * was causing 2 observers to be created, ensure
 * that after removal of the mediator there will
 * be no further response.
 */
void testMediatorReregistration() {
    // Get the Singleton View instance
    View *view = $View.getInstance("ViewTestKey10", $View.new);

    // Create and register that responds to notification 5
    ViewTest viewTest = {};
    Mediator *mediator = (Mediator *) NewViewTestMediator5(&viewTest);

    // try to register another instance of that mediator (uses the same NAME constant).
    view->registerMediator(view, mediator);

    // test that the counter is only incremented once (mediator 5's response)
    viewTest.counter = 0;
    Notification *notification = $Notification.new(NOTE5, NULL, NULL);
    view->notifyObservers(view, notification);
    assert(viewTest.counter == 1);

    // Remove the Mediator
    Mediator *removed = view->removeMediator(view, ViewTestMediator5_NAME);
    $Mediator.delete(removed);

    // test that retrieving it now returns null
    assert(view->retrieveMediator(view, ViewTestMediator5_NAME) == NULL);

    // test that the counter is no longer incremented
    viewTest.counter = 0;
    view->notifyObservers(view, notification);
    assert(viewTest.counter == 0);

    $Notification.delete(notification);
    $View.removeView("ViewTestKey10");
    view = NULL;
}

/**
 * Tests the ability for the observer list to
 * be modified during the process of notification,
 * and all observers be properly notified. This
 * happens most often when multiple Mediators
 * respond to the same notification by removing
 * themselves.
 */
void testModifyObserverListDuringNotification() {
    // Get the Singleton View instance
    View *view = $View.getInstance("ViewTestKey11", $View.new);

    ViewTest viewTest = {"", "", "", 0};

    // Create and register several mediator instances that respond to notification 6
    // by removing themselves, which will cause the observer list for that notification
    // to change.
    view->registerMediator(view, (Mediator *) NewViewTestMediator6("ViewTestMediator6/1", &viewTest));
    view->registerMediator(view, (Mediator *) NewViewTestMediator6("ViewTestMediator6/2", &viewTest));
    view->registerMediator(view, (Mediator *) NewViewTestMediator6("ViewTestMediator6/3", &viewTest));
    view->registerMediator(view, (Mediator *) NewViewTestMediator6("ViewTestMediator6/4", &viewTest));
    view->registerMediator(view, (Mediator *) NewViewTestMediator6("ViewTestMediator6/5", &viewTest));
    view->registerMediator(view, (Mediator *) NewViewTestMediator6("ViewTestMediator6/6", &viewTest));
    view->registerMediator(view, (Mediator *) NewViewTestMediator6("ViewTestMediator6/7", &viewTest));
    view->registerMediator(view, (Mediator *) NewViewTestMediator6("ViewTestMediator6/8", &viewTest));

    // send the notification. each of the above mediators will respond by removing
    // themselves and incrementing the counter by 1. This should leave us with a
    // count of 8, since 8 mediators will respond.
    Notification *notification = $Notification.new(NOTE6, NULL, NULL);
    view->notifyObservers(view, notification);

    // verify the count is correct
    assert(viewTest.counter == 8);

    // clear the counter
    viewTest.counter = 0;
    view->notifyObservers(view, notification);

    // verify the count is 0
    assert(viewTest.counter == 0);

    $Notification.delete(notification);
    $Facade.removeFacade("ViewTestKey11");
    view = NULL;
}

void testRemoveView() {
    // Get a Multiton View instance
    $View.getInstance("ViewTestKey12", $View.new);

    // remove the View
    $View.removeView("ViewTestKey12");

    // re-create the view without throwing an exception
    $View.new("ViewTestKey12");

    // cleanup
    $View.removeView("ViewTestKey12");
}
