#include "ObserverTest.h"
#include "interfaces/Observer.h"
#include <assert.h>
#include <stdio.h>

int main() {
    testObserverAccessors();
    testObserverConstructor();
    testCompareNotifyContext();
    puts("ObserverTest: Success");
    return 0;
}

typedef struct ObserverTestVar {
    int value;
} ObserverTestVar;

ObserverTestVar observerTestVar = {0};

void testObserverAccessors() {
    Observer *observer = NewObserver(NULL, NULL);

    observer->setNotifyMethod(observer, observerTestMethod);
    observer->setNotifyContext(observer, &observerTestVar);

    ObserverTestVar temp = {10};
    Notification *notification = NewNotification("ObserverTestNote", &temp, NULL);
    observer->notifyObserver(observer, notification);

    assert(observerTestVar.value == 10);

    DeleteNotification(notification);
    DeleteObserver(observer);
}

void testObserverConstructor() {
    ObserverTestVar temp = {5};
    Observer *observer = NewObserver(observerTestMethod, &temp);
    Notification *notification = NewNotification("ObserverTestNote", &temp, NULL);
    observer->notifyObserver(observer, notification);

    assert(observerTestVar.value == 5);

    DeleteNotification(notification);
    DeleteObserver(observer);
}

void testCompareNotifyContext() {
    ObserverTestVar observerTestVar = {};
    Observer *observer = NewObserver(observerTestMethod, &observerTestVar);

    void *negTestObj = (void *)8;
    assert(observer->compareNotifyContext(observer, negTestObj) == false);
    assert(observer->compareNotifyContext(observer, &observerTestVar) == true);

    DeleteObserver(observer);
}

static void observerTestMethod(void *context, Notification *notification) {
    observerTestVar = *(ObserverTestVar *) notification->getBody(notification);
}