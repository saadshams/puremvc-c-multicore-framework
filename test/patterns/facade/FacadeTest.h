#ifndef PUREMVC_FACADETEST_H
#define PUREMVC_FACADETEST_H

void testGetInstance();
void testRegisterCommandAndSendNotification();
void testRegisterAndRemoveCommandAndSendNotification();
void testRegisterAndRetrieveProxy();
void testRegisterAndRemoveProxy();
void testRegisterRetrieveAndRemoveMediator();
void testHasProxy();
void testHasMediator();
void testHasCommand();
void testHasCoreAndRemoveCore();
void testGetInstancesThreaded();

#endif //PUREMVC_FACADETEST_H
