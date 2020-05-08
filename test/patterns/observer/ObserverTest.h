#ifndef PUREMVC_OBSERVERTEST_H
#define PUREMVC_OBSERVERTEST_H

#include "interfaces/Notification.h"

void testObserverAccessors();

void testObserverConstructor();

void testCompareNotifyContext();

void testCompareNotifyContext();

static void observerTestMethod(void *context, Notification *notification);

#endif //PUREMVC_OBSERVERTEST_H