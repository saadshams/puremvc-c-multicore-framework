#ifndef PUREMVC_OBSERVERTEST_H
#define PUREMVC_OBSERVERTEST_H

#include "interfaces/Notification.h"

typedef struct {
    int value;
} ObserverTestVar;

int main(int argc, char *argv[]);

void testObserverAccessors();

void testObserverConstructor();

void testCompareNotifyContext();

#endif //PUREMVC_OBSERVERTEST_H