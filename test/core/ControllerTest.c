#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "puremvc/puremvc.h"

#include "ControllerTest.h"
#include "ControllerTestCommand.h"
#include "ControllerTestCommand2.h"
#include "ControllerTestVO.h"

int main() {
    testGetInstance();
    testRegisterAndExecuteCommand();
    testRegisterAndRemoveCommand();
    testHasCommand();
    testReregisterAndExecuteCommand();
    testRegisterAndUpdateCommand();
    testRemoveController();
    return 0;
}

void testGetInstance() {
    // Test Factory Method
    const char *error = NULL;
    const struct IController *controller = puremvc_controller_getInstance("ControllerTestKey1", puremvc_controller_new, &error);

    // test assertions
    assert(controller != NULL);
    assert(controller == puremvc_controller_getInstance("ControllerTestKey1", puremvc_controller_new, &error));
    puremvc_controller_removeController("ControllerTestKey1");
    controller = NULL;
}

void testRegisterAndExecuteCommand() {
    // Create the controller, register the ControllerTestCommand to handle 'ControllerTest' notes
    const char *error = NULL;
    const struct IController *controller = puremvc_controller_getInstance("ControllerTestKey2", puremvc_controller_new, &error);
    controller->registerCommand(controller, "ControllerTest1", test_controller_command_new, &error);

    // Create a 'ControllerTest' note
    struct ControllerTestVO *vo = malloc(sizeof(struct ControllerTestVO));
    *vo = (struct ControllerTestVO){12, 0};
    struct INotification *notification = puremvc_notification_new("ControllerTest1", vo, NULL, &error);

    // Tell the controller to execute the Command associated with the note
    // the ControllerTestCommand invoked will multiply the vo.input value
    // by 2 and set the result on vo.result
    controller->executeCommand(controller, notification, &error);

    // test assertions
    assert(vo->result == 24);

    controller->removeCommand(controller, "ControllerTest1");
    puremvc_notification_free(&notification);
    puremvc_controller_removeController("ControllerTestKey2");
    controller = NULL;
}

void testRegisterAndRemoveCommand() {
    // Create the controller, register the ControllerTestCommand to handle 'ControllerTest' notes
    const char *error = NULL;
    const struct IController *controller = puremvc_controller_getInstance("ControllerTestKey3", puremvc_controller_new, &error);
    controller->registerCommand(controller, "ControllerRemoveTest", test_controller_command_new, &error);

    // Create a 'ControllerTest' note
    struct ControllerTestVO *vo = malloc(sizeof(struct ControllerTestVO));
    *vo = (struct ControllerTestVO){12, 0};
    struct INotification *notification = puremvc_notification_new("ControllerRemoveTest", vo, NULL, &error);

    // Tell the controller to execute the Command associated with the note
    // the ControllerTestCommand invoked will multiply the vo.input value
    // by 2 and set the result on vo.result
    controller->executeCommand(controller, notification, &error);

    // test assertions
    assert(vo->result == 24);

    // Reset result
    vo->result = 0;

    // Remove the Command from the Controller
    controller->removeCommand(controller, "ControllerRemoveTest");

    // Tell the controller to execute the Command associated with the
    // note. This time, it should not be registered, and our vo result
    // will not change
    controller->executeCommand(controller, notification, &error);

    // test assertions
    assert(vo->result == 0);
    puremvc_notification_free(&notification);
    puremvc_controller_removeController("ControllerTestKey3");
    controller = NULL;
}

void testHasCommand() {
    // register the ControllerTestCommand to handle 'hasCommandTest' notes
    const char *error = NULL;
    const struct IController *controller = puremvc_controller_getInstance("ControllerTestKey4", puremvc_controller_new, &error);

    // test that hasCommand returns true for hasCommandTest notifications
    controller->registerCommand(controller, "hasCommandTest", test_controller_command_new, &error);
    assert(controller->hasCommand(controller, "hasCommandTest"));

    // Remove the Command from the Controller
    controller->removeCommand(controller, "hasCommandTest");

    // test that hasCommand returns false for hasCommandTest notifications
    assert(controller->hasCommand(controller, "hasCommandTest") == false);

    puremvc_controller_removeController("ControllerTestKey4");
    controller = NULL;
}

void testReregisterAndExecuteCommand() {
    // Fetch the controller, register the ControllerTestCommand2 to handle 'ControllerTest2' notes
    const char *error = NULL;
    const struct IController *controller = puremvc_controller_getInstance("ControllerTestKey5", puremvc_controller_new, &error);
    controller->registerCommand(controller, "ControllerTest2", test_controller_command2_new, &error);

    // Remove the Command from the Controller
    controller->removeCommand(controller, "ControllerTest2");

    // Re-register the Command with the Controller
    controller->registerCommand(controller, "ControllerTest2", test_controller_command2_new, &error);

    // Create a 'ControllerTest2' note
    struct ControllerTestVO *vo = malloc(sizeof(struct ControllerTestVO));
    *vo = (struct ControllerTestVO){12, 0};
    struct INotification *notification = puremvc_notification_new("ControllerTest2", vo, NULL, &error);

    // retrieve a reference to the View from the same core.
    struct IView *view = puremvc_view_getInstance("ControllerTestKey5", puremvc_view_new, &error);
    view->notifyObservers(view, notification, &error);

    // test assertions
    // if the command is executed once the value will be 24
    assert(vo->result == 24);

    // Prove that accumulation works in the VO by sending the notification again
    view->notifyObservers(view, notification, &error);

    // if the command is executed twice the value will be 48
    assert(vo->result == 48);

    controller->removeCommand(controller, "ControllerTest2");
    puremvc_notification_free(&notification);
    puremvc_controller_removeController("ControllerTestKey5");
    puremvc_view_removeView("ControllerTestKey5");
    controller = NULL;
}

void testRegisterAndUpdateCommand() {
    const char *error = NULL;
    const struct IController *controller = puremvc_controller_getInstance("ControllerTestKey3_2", puremvc_controller_new, &error);

    // first registration
    controller->registerCommand(controller, "ControllerTest2", test_controller_command_new, &error);

    // update command
    controller->registerCommand(controller, "ControllerTest2", test_controller_command2_new, &error);

    struct ControllerTestVO *vo = malloc(sizeof(struct ControllerTestVO));
    *vo = (struct ControllerTestVO){12, 10};
    struct INotification *notification = puremvc_notification_new("ControllerTest2", vo, NULL, &error);
    controller->executeCommand(controller, notification, &error);

    // second command result
    assert(vo->result == 34);

    controller->removeCommand(controller, "ControllerTest2");
    puremvc_notification_free(&notification);
    puremvc_controller_removeController("ControllerTestKey3_2");
    controller = NULL;
}

void testRemoveController() {
    // Get a Multiton Controller instance
    const char *error = NULL;
    puremvc_controller_getInstance("ControllerTestKey4", puremvc_controller_new, &error);

    // remove the controller
    puremvc_controller_removeController("ControllerTestKey4");

    // re-create the controller without throwing an exception
    puremvc_controller_new("ControllerTestKey4", &error);

    // cleanup
    puremvc_controller_removeController("ControllerTestKey4");
}
