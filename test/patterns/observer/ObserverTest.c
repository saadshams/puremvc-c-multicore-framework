#include "ObserverTest.h"
#include "interfaces/Observer.h"
#include <assert.h>
#include <stdio.h>

/**
 * Tests PureMVC Observer class.
 *
 * <P>Since the Observer encapsulates the interested object's
 * callback information, there are no getters, only setters.
 * It is, in effect write-only memory.</P>
 *
 * <P>Therefore, the only way to test it is to set the
 * notification method and context and call the notifyObserver
 * method.</P>
 *
 */
int main() {
    testObserverAccessors();
    testObserverConstructor();
    testCompareNotifyContext();
    puts("ObserverTest: Success");
    return 0;
}

/**
 * A test variable that proves the notify method was
 * executed with 'handleNotification' as its execution context
 */
ObserverTestVar observerTestVar = {0};

/**
 * A function that is used as the observer notification
 * method.
 */
static void handleNotification(void *context, Notification *notification) {
    observerTestVar = *(ObserverTestVar *) notification->getBody(notification);
}

/**
 * Tests observer class when initialized by accessor methods.
 */
void testObserverAccessors() {
    // Create observer with null args, then
    // use accessors to set notification method and context
    struct Object {} object;
    Observer *observer = NewObserver(NULL, NULL);
    observer->setNotifyMethod(observer, handleNotification);
    observer->setNotifyContext(observer, &object);

    // create a test event, setting a payload value and notify
    // the observer with it. since the observer is this class
    // and the notification method is observerTestMethod,
    // successful notification will result in our local
    // observerTestVar being set to the value we pass in
    // on the note body.
    Notification *notification = NewNotification("ObserverTestNote", &(ObserverTestVar){10}, NULL);
    observer->notifyObserver(observer, notification);

    // test assertions
    assert(observerTestVar.value == 10);

    DeleteNotification(notification);
    DeleteObserver(observer);
}

/**
 * Tests the Constructor method of the Observer class
 */
void testObserverConstructor() {
    // Create observer
    struct Object {} object;
    Observer *observer = NewObserver(handleNotification, &object);
    Notification *notification = NewNotification("ObserverTestNote", &(ObserverTestVar){5}, NULL);
    observer->notifyObserver(observer, notification);

    // test assertions
    assert(observerTestVar.value == 5);

    DeleteNotification(notification);
    DeleteObserver(observer);
}

/**
 * Tests the compareNotifyContext method of the Observer class
 */
void testCompareNotifyContext() {
    // Create observer passing in notification method and context
    struct Object {} object, negTestObj;
    Observer *observer = NewObserver(handleNotification, &object);

    // test assertions
    assert(observer->compareNotifyContext(observer, &negTestObj) == false);
    assert(observer->compareNotifyContext(observer, &object) == true);

    DeleteObserver(observer);
}
