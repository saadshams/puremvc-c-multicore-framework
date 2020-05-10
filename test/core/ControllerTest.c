#include "ControllerTest.h"
#include "interfaces/Controller.h"
#include <assert.h>
#include "ControllerTestCommand.h"
#include "ControllerTestCommand2.h"
#include "ControllerTestVO.h"
#include <stdio.h>

int main() {
    testGetInstance();
    testRegisterAndExecuteCommand();
    testRegisterAndRemoveCommand();
    testHasCommand();
    testReregisterAndExecuteCommand();
    testRegisterAndUpdateCommand();
    puts("ControllerTest: Success");
    return 0;
}

void testGetInstance() {
    Controller *controller = getControllerInstance("ControllerTestKey1", NewController);
    assert(controller != NULL);
    assert(controller == getControllerInstance("ControllerTestKey1", NewController));
    DeleteController("ControllerTestKey1");
}

void testRegisterAndExecuteCommand() {
    Controller *controller = getControllerInstance("ControllerTestKey2", NewController);
    controller->registerCommand(controller, "ControllerTest1", (SimpleCommand *(*)()) NewControllerTestCommand);

    ControllerTestVO *vo = NewControllerTestVO(12);
    Notification *notification = NewNotification("ControllerTest1", vo, NULL);
    controller->executeCommand(controller, notification);

    assert(vo->result == 24);

    controller->removeCommand(controller, "ControllerTest1");
    DeleteControllerTestVO(vo);
    DeleteNotification(notification);
    DeleteController("ControllerTestKey2");
}

void testRegisterAndRemoveCommand() {
    Controller *controller = getControllerInstance("ControllerTestKey3", NewController);
    controller->registerCommand(controller, "ControllerRemoveTest", (SimpleCommand *(*)()) NewControllerTestCommand);

    ControllerTestVO *vo = NewControllerTestVO(12);
    Notification *notification = NewNotification("ControllerRemoveTest", vo, NULL);
    controller->executeCommand(controller, notification);
    assert(vo->result == 24);

    vo->result = 0;
    controller->removeCommand(controller, "ControllerRemoveTest");
    controller->executeCommand(controller, notification);

    assert(vo->result == 0);
    DeleteNotification(notification);
    DeleteControllerTestVO(vo);
    DeleteController("ControllerTestKey3");
}

void testHasCommand() {
    Controller *controller = getControllerInstance("ControllerTestKey4", NewController);

    controller->registerCommand(controller, "hasCommandTest", (SimpleCommand *(*)()) NewControllerTestCommand);
    assert(controller->hasCommand(controller, "hasCommandTest"));

    controller->removeCommand(controller, "hasCommandTest2");
    assert(controller->hasCommand(controller, "hasCommandTest2") == false);

    controller->removeCommand(controller, "hasCommandTest");
    assert(controller->hasCommand(controller, "hasCommandTest") == false);

    controller->registerCommand(controller, "Test1", NewSimpleCommand);
    controller->registerCommand(controller, "Test2", NewSimpleCommand);
    controller->registerCommand(controller, "Test3", NewSimpleCommand);
    controller->registerCommand(controller, "Test4", NewSimpleCommand);

    // test hasCommand at random
    assert(controller->hasCommand(controller, "None") == false); // non existing
    assert(controller->hasCommand(controller, "Test4") == true); // tail
    assert(controller->hasCommand(controller, "Test1") == true); // head
    assert(controller->hasCommand(controller, "Test3") == true); // mid
    assert(controller->hasCommand(controller, "Test2") == true); // mid

    // remove commands at random
    controller->removeCommand(controller, "Test4"); // tail
    controller->removeCommand(controller, "Test1"); // head
    controller->removeCommand(controller, "Test3"); // mid
    controller->removeCommand(controller, "Test2"); // mid

    assert(controller->hasCommand(controller, "Test4") == false); // tail
    assert(controller->hasCommand(controller, "Test1") == false); // head
    assert(controller->hasCommand(controller, "Test3") == false); // mid
    assert(controller->hasCommand(controller, "Test2") == false); // mid

    DeleteController("ControllerTestKey4");
}

void testReregisterAndExecuteCommand() {
    Controller *controller = getControllerInstance("ControllerTestKey5", NewController);
    controller->registerCommand(controller, "ControllerTest3", (SimpleCommand *(*)()) NewControllerTestCommand);
    controller->registerCommand(controller, "ControllerTest3", (SimpleCommand *(*)()) NewControllerTestCommand2);

    ControllerTestVO *vo = NewControllerTestVO(12);
    Notification *notification = NewNotification("ControllerTest3", vo, NULL);
    controller->executeCommand(controller, notification);
    assert(vo->result == 24);

    controller->removeCommand(controller, "ControllerTest3");
    DeleteControllerTestVO(vo);
    DeleteNotification(notification);

    controller->registerCommand(controller, "ControllerTest3", (SimpleCommand *(*)()) NewControllerTestCommand);
    controller->removeCommand(controller, "ControllerTest3");
    controller->registerCommand(controller, "ControllerTest3", (SimpleCommand *(*)()) NewControllerTestCommand2);

    vo = NewControllerTestVO(12);
    notification = NewNotification("ControllerTest3", vo, NULL);

    controller->removeCommand(controller, "ControllerTest3");
    DeleteControllerTestVO(vo);
    DeleteNotification(notification);
    DeleteController("ControllerTestKey5");
}

void testRegisterAndUpdateCommand() {
    Controller *controller = getControllerInstance("ControllerTestKey3_2", NewController);

    controller->registerCommand(controller, "ControllerTest2", (SimpleCommand *(*)()) NewControllerTestCommand);
    controller->registerCommand(controller, "ControllerTest2", (SimpleCommand *(*)()) NewControllerTestCommand2);

    ControllerTestVO *vo = NewControllerTestVO(12);
    vo->result = 10;
    Notification *notification = NewNotification("ControllerTest2", vo, NULL);
    controller->executeCommand(controller, notification);
    assert(vo->result == 34);

    controller->removeCommand(controller, "ControllerTest2");
    DeleteNotification(notification);
    DeleteControllerTestVO(vo);
    DeleteController("ControllerTestKey3_2");
}

