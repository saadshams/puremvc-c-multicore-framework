#ifndef PUREMVC_VIEWTEST_H
#define PUREMVC_VIEWTEST_H

#include <stdbool.h>

typedef struct ViewTest ViewTest;

struct ViewTest {
    const char *lastNotification;
    bool onRegisterCalled;
    bool onRemoveCalled;
    int counter;
};

static char *NOTE1 = "Notification11";
static char *NOTE2 = "Notification21";
static char *NOTE3 = "Notification3";
static char *NOTE4 = "Notification4";
static char *NOTE5 = "Notification5";
static char *NOTE6 = "Notification6";

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
