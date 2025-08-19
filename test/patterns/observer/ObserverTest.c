#include <assert.h>
#include <stdlib.h>

#include "puremvc/IObserver.h"

struct ObserverTestVar {
    int value;
};

/**
 * A test variable that proves the notify method was
 * executed with 'handleNotification' as its execution context
 */
static struct ObserverTestVar observerTestVar = {0};

/**
 * A function that is used as the observer notification
 * method.
 */
static void handleNotification(const void *context, struct INotification *notification) {
    observerTestVar = *(struct ObserverTestVar *) notification->getBody(notification);
}

/**
 * Tests the Constructor method of the Observer class
 */
void testObserverConstructor() {
    // Create observer
    struct Object {} object;
    struct IObserver *observer = puremvc_observer_new((const void (*)(const void *, struct INotification *))handleNotification, &object);

    struct ObserverTestVar *var = malloc(sizeof(struct ObserverTestVar));
    if (var) *var = (struct ObserverTestVar){ .value = 5 };
    struct INotification *notification = puremvc_notification_new("ObserverTestNote", var, NULL);
    observer->notifyObserver(observer, notification);

    // test assertions
    assert(observerTestVar.value == 5);

    puremvc_notification_free(&notification);
    assert(notification == NULL);
    puremvc_observer_free(&observer);
    assert(observer == NULL);
}

/**
 * Tests observer class when initialized by accessor methods.
 */
void testObserverAccessors() {
    // Create observer with null args, then
    // use accessors to set notification method and context
    struct Object {} object;
    struct IObserver *observer = puremvc_observer_new(NULL, NULL);
    observer->setContext(observer, &object);
    observer->setNotify(observer, handleNotification);

    assert(observer->getContext(observer) == &object);
    assert(observer->getNotify(observer) == handleNotification);

    // create a test event, setting a payload value and notify
    // the observer with it. since the observer is this class
    // and the notification method is observerTestMethod,
    // successful notification will result in our local
    // observerTestVar being set to the value we pass in
    // on the note body.
    struct ObserverTestVar *vo = malloc(sizeof(struct ObserverTestVar));
    if (vo) *vo = (struct ObserverTestVar){ .value = 10 };
    struct INotification *notification = puremvc_notification_new("ObserverTestNote", vo, NULL);
    observer->notifyObserver(observer, notification);

    puremvc_notification_free(&notification);
    assert(notification == NULL);
    puremvc_observer_free(&observer);
    assert(observer == NULL);
}

/**
 * Tests the compareNotifyContext method of the Observer class
 */
void testCompareNotifyContext() {
    // Create observer passing in notification method and context
    struct Object {char dummy;};
    const struct Object object = {0};
    const struct Object negTestObj = {0};
    struct IObserver *observer = puremvc_observer_new((const void (*)(const void *, struct INotification *))handleNotification, (void *)&object);

    // test assertions
    assert(observer->compareNotifyContext(observer, &negTestObj) == false);
    assert(observer->compareNotifyContext(observer, &object) == true);

    puremvc_observer_free(&observer);
    assert(observer == NULL);
}

int main() {
    testObserverConstructor();
    testObserverAccessors();
    testCompareNotifyContext();
    return 0;
}
