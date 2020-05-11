#ifndef PUREMVC_VIEWTEST_H
#define PUREMVC_VIEWTEST_H

#include <stdbool.h>

typedef struct ViewTest ViewTest;

struct ViewTest {
    char *lastNotification;
    bool onRegisterCalled;
    bool onRemoveCalled;
    int counter;
};

#define NOTE1 "Notification1"
#define NOTE2 "Notification2"
#define NOTE3 "Notification3"
#define NOTE4 "Notification4"
#define NOTE5 "Notification5"
#define NOTE6 "Notification6"

void testGetInstance();
void testRegisterAndNotifyObserver();
void testRegisterAndRetrieveMediator();
void testHasMediator();
void testRegisterAndRemoveMediator();
void testOnRegisterAndOnRemove();
void testSuccessiveRegisterAndRemoveMediator();
void testRemoveMediatorAndSubsequentNotify();
void testRemoveOneOfTwoMediatorsAndSubsequentNotify();
void testMediatorReregistration();
void testModifyObserverListDuringNotification();

#endif //PUREMVC_VIEWTEST_H
