#include "ControllerTest.h"
#include "interfaces/Controller.h"
#include "ControllerTestCommand.h"
#include "ControllerTestCommand2.h"
#include "ControllerTestVO.h"
#include <assert.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    testGetInstance();
    testRegisterAndExecuteCommand();
    testRegisterAndRemoveCommand();
    testHasCommand();
    testReregisterAndExecuteCommand();
    testRegisterAndUpdateCommand();
    testRemoveController();
    puts("ControllerTest: Success");
    return 0;
}

/**
 * Tests the Controller Multiton Factory Method
 */
void testGetInstance() {
    // Test Factory Method
    Controller *controller = $Controller.getInstance("ControllerTestKey1", $Controller.new);

    // test assertions
    assert(controller != NULL);
    assert(controller == $Controller.getInstance("ControllerTestKey1", $Controller.new));
    $Controller.removeController("ControllerTestKey1");
    controller = NULL;
}

/**
 * Tests Command registration and execution.
 *
 * <P>This test gets a Multiton Controller instance
 * and registers the ControllerTestCommand class
 * to handle 'ControllerTest' Notifications.<P>
 *
 * <P>It then constructs such a Notification and tells the
 * Controller to execute the associated Command.
 * Success is determined by evaluating a property
 * on an object passed to the Command, which will
 * be modified when the Command executes.</P>
 */
void testRegisterAndExecuteCommand() {
    // Create the controller, register the ControllerTestCommand to handle 'ControllerTest' notes
    Controller *controller = $Controller.getInstance("ControllerTestKey2", $Controller.new);
    controller->registerCommand(controller, "ControllerTest1", NewControllerTestCommand);

    // Create a 'ControllerTest' note
    ControllerTestVO vo = {12, 0};
    Notification *notification = $Notification.new("ControllerTest1", &vo, NULL);

    // Tell the controller to execute the Command associated with the note
    // the ControllerTestCommand invoked will multiply the vo.input value
    // by 2 and set the result on vo.result
    controller->executeCommand(controller, notification);

    // test assertions
    assert(vo.result == 24);

    controller->removeCommand(controller, "ControllerTest1");
    $Notification.delete(notification);
    $Controller.removeController("ControllerTestKey2");
    controller = NULL;
}

/**
 * Tests Command registration and removal.
 *
 * <P>Tests that once a Command is registered and verified
 * working, it can be removed from the Controller.</P>
 */
void testRegisterAndRemoveCommand() {
    // Create the controller, register the ControllerTestCommand to handle 'ControllerTest' notes
    Controller *controller = $Controller.getInstance("ControllerTestKey3", $Controller.new);
    controller->registerCommand(controller, "ControllerRemoveTest", NewControllerTestCommand);

    // Create a 'ControllerTest' note
    ControllerTestVO vo = {12, 0};
    Notification *notification = $Notification.new("ControllerRemoveTest", &vo, NULL);

    // Tell the controller to execute the Command associated with the note
    // the ControllerTestCommand invoked will multiply the vo.input value
    // by 2 and set the result on vo.result
    controller->executeCommand(controller, notification);

    // test assertions
    assert(vo.result == 24);

    // Reset result
    vo.result = 0;

    // Remove the Command from the Controller
    controller->removeCommand(controller, "ControllerRemoveTest");

    // Tell the controller to execute the Command associated with the
    // note. This time, it should not be registered, and our vo result
    // will not change
    controller->executeCommand(controller, notification);

    // test assertions
    assert(vo.result == 0);
    $Notification.delete(notification);
    $Controller.removeController("ControllerTestKey3");
    controller = NULL;
}

/**
 * Test hasCommand method.
 */
void testHasCommand() {
    // register the ControllerTestCommand to handle 'hasCommandTest' notes
    Controller *controller = $Controller.getInstance("ControllerTestKey4", $Controller.new);

    // test that hasCommand returns true for hasCommandTest notifications
    controller->registerCommand(controller, "hasCommandTest", NewControllerTestCommand);
    assert(controller->hasCommand(controller, "hasCommandTest"));

    // Remove the Command from the Controller
    controller->removeCommand(controller, "hasCommandTest");

    // test that hasCommand returns false for hasCommandTest notifications
    assert(controller->hasCommand(controller, "hasCommandTest") == false);

    $Controller.removeController("ControllerTestKey4");
    controller = NULL;
}

/**
 * Tests Removing and Reregistering a Command
 *
 * <P>Tests that when a Command is re-registered that it isn't fired twice.
 * This involves, minimally, registration with the controller but
 * notification via the View, rather than direct execution of
 * the Controller's executeCommand method as is done above in
 * testRegisterAndRemove. </P>
 */
void testReregisterAndExecuteCommand() {
    // Fetch the controller, register the ControllerTestCommand2 to handle 'ControllerTest2' notes
    Controller *controller = $Controller.getInstance("ControllerTestKey5", $Controller.new);
    controller->registerCommand(controller, "ControllerTest2", NewControllerTestCommand2);

    // Remove the Command from the Controller
    controller->removeCommand(controller, "ControllerTest2");

    // Re-register the Command with the Controller
    controller->registerCommand(controller, "ControllerTest2", NewControllerTestCommand2);

    // Create a 'ControllerTest2' note
    ControllerTestVO vo = {12, 0};
    Notification *notification = $Notification.new("ControllerTest2", &vo, NULL);

    // retrieve a reference to the View from the same core.
    View *view = $View.getInstance("ControllerTestKey5", $View.new);
    view->notifyObservers(view, notification);

    // test assertions
    // if the command is executed once the value will be 24
    assert(vo.result == 24);

    // Prove that accumulation works in the VO by sending the notification again
    view->notifyObservers(view, notification);

    // if the command is executed twice the value will be 48
    assert(vo.result == 48);

    controller->removeCommand(controller, "ControllerTest2");
    $Notification.delete(notification);
    $Controller.removeController("ControllerTestKey5");
    $View.removeView("ControllerTestKey5");
    controller = NULL;
}

/**
 * Test register a command, and then update the same notification with a second command
 */
void testRegisterAndUpdateCommand() {
    Controller *controller = $Controller.getInstance("ControllerTestKey3_2", $Controller.new);

    // first registration
    controller->registerCommand(controller, "ControllerTest2", NewControllerTestCommand);

    // update command
    controller->registerCommand(controller, "ControllerTest2", NewControllerTestCommand2);

    ControllerTestVO vo = {12, 10};
    Notification *notification = $Notification.new("ControllerTest2", &vo, NULL);
    controller->executeCommand(controller, notification);

    // second command result
    assert(vo.result == 34);

    controller->removeCommand(controller, "ControllerTest2");
    $Notification.delete(notification);
    $Controller.removeController("ControllerTestKey3_2");
    controller = NULL;
}

void testRemoveController() {
    // Get a Multiton Controller instance
    $Controller.getInstance("ControllerTestKey4", $Controller.new);

    // remove the controller
    $Controller.removeController("ControllerTestKey4");

    // re-create the controller without throwing an exception
    $Controller.new("ControllerTestKey4");

    // cleanup
    $Controller.removeController("ControllerTestKey4");
}
