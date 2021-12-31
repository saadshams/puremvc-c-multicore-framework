#include "MacroCommandTest.h"
#include "MacroCommandTestVO.h"
#include "interfaces/Notification.h"
#include "interfaces/Notifier.h"
#include "MacroCommandTestCommand.h"
#include <assert.h>
#include <stdio.h>

int main() {
    testMacroCommandExecute();
    testRegisterAndExecuteCommand();
    puts("MacroCommandTest: Success");
    return 0;
}

/**
 * Tests operation of a <code>MacroCommand</code>.
 *
 * <P>This test creates a new <code>Notification</code>, adding a
 * <code>MacroCommandTestVO</code> as the body.
 * It then creates a <code>MacroCommandTestCommand</code> and invokes
 * its <code>execute</code> method, passing in the
 * <code>Notification</code>.<P>
 *
 * <P>The <code>MacroCommandTestCommand</code> has defined an
 * <code>initializeMacroCommand</code> method, which is
 * called automatically by its constructor. In this method
 * the <code>MacroCommandTestCommand</code> adds 2 SubCommands
 * to itself, <code>MacroCommandTestSub1Command</code> and
 * <code>MacroCommandTestSub2Command</code>.
 *
 * <P>The <code>MacroCommandTestVO</code> has 2 result properties,
 * one is set by <code>MacroCommandTestSub1Command</code> by
 * multiplying the input property by 2, and the other is set
 * by <code>MacroCommandTestSub2Command</code> by multiplying
 * the input property by itself.
 *
 * <P>Success is determined by evaluating the 2 result properties
 * on the <code>MacroCommandTestVO</code> that was passed to
 * the <code>MacroCommandTestCommand</code> on the Notification
 * body.</P>
 */
void testMacroCommandExecute() {
    // Create the VO
    MacroCommandTestVO vo = {5, 0, 0};

    // Create the Notification (note)
    Notification *notification = NewNotification("MacroCommandTest", &vo, NULL);

    // Create the SimpleCommand
    MacroCommand *command = NewMacroCommandTestCommand();
    command->simpleCommand.notifier->initializeNotifier(command->simpleCommand.notifier, "test");

    // Execute the SimpleCommand
    command->simpleCommand.execute(&command->simpleCommand, notification);

    // test assertions
    assert(vo.result1 == 10);
    assert(vo.result2 == 25);

    DeleteNotification(notification);
    DeleteMacroCommand(command);
}

/**
 * Tests Command registration and execution.
 *
 * <P>This test gets a Multiton Controller instance
 * and registers the MacroCommandTestCommand class
 * to handle 'ControllerTest' Notifications.<P>
 *
 * <P>It then constructs such a Notification and tells the
 * View to execute the associated Command.
 * Success is determined by evaluating a property
 * on an object passed to the Command, which will
 * be modified when the Command executes.</P>
 */
void testRegisterAndExecuteCommand() {
    // Create the controller, register the ControllerTestCommand to handle 'ControllerTest' notes
    Controller *controller = getControllerInstance("ControllerTestKey1", NewController);
    controller->registerCommand(controller, "ControllerTest", (SimpleCommand *(*)(void)) NewMacroCommandTestCommand);

    View *view = getViewInstance("ControllerTestKey1", NewView);

    // Create a 'ControllerTest' note
    MacroCommandTestVO vo = {5, 0, 0};
    Notification *notification = NewNotification("ControllerTest", &vo, NULL);

    view->notifyObservers(view, notification);

    // test assertions
    assert(vo.result1 == 10);
    assert(vo.result2 == 25);

    controller->removeCommand(controller, "ControllerTest1");
    DeleteNotification(notification);
    RemoveController("ControllerTestKey2");
    controller = NULL;
}