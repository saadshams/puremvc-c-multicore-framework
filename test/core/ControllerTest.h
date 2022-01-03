#ifndef PUREMVC_CONTROLLERTEST_H
#define PUREMVC_CONTROLLERTEST_H

/**
 * Test the PureMVC Controller class.
 *
 * @see ControllerTestVO
 * @see ControllerTestCommand
 */
void testGetInstance();

void testRegisterAndExecuteCommand();

void testRegisterAndRemoveCommand();

void testHasCommand();

void testReregisterAndExecuteCommand();

void testRegisterAndUpdateCommand();

void testRemoveController();

#endif //PUREMVC_CONTROLLERTEST_H
