#ifndef PUREMVC_OBSERVERTEST_H
#define PUREMVC_OBSERVERTEST_H

#include "interfaces/Notification.h"

typedef struct {
    int value;
} ObserverTestVar;

void testObserverAccessors();

void testObserverConstructor();

void testCompareNotifyContext();

void testCompareNotifyContext();

#endif //PUREMVC_OBSERVERTEST_H