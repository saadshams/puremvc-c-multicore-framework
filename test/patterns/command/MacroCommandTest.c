#include <assert.h>
#include <stdlib.h>

#include "puremvc/puremvc.h"

#include "MacroCommandTest.h"
#include "MacroCommandTestCommand.h"
#include "MacroCommandTestVO.h"

int main() {
    testMacroCommandExecute();
    testRegisterAndExecuteCommand();
    return 0;
}

void testMacroCommandExecute() {
    struct MacroCommandTestVO *vo = malloc(sizeof(struct MacroCommandTestVO));
    if (vo) *vo = (struct MacroCommandTestVO){5, 0, 0};

    struct INotification *notification = puremvc_notification_new("MacroCommandTest", vo, NULL);

    struct IMacroCommand *macroCommand = macro_command_test_command_new();
    macroCommand->command.notifier->initializeNotifier(macroCommand->command.notifier, "MacroCommandTestkey1");
    macroCommand->command.execute(&macroCommand->command, notification);

    assert(vo->result1 == 10);

    assert(vo->result2 == 25);

    puremvc_macro_command_free(&macroCommand);
}

void testRegisterAndExecuteCommand() {
    const struct IController *controller = puremvc_controller_getInstance("ControllerTestKey1", puremvc_controller_new);
    controller->registerCommand(controller, "MacroCommandTest", (struct ICommand *(*)(void)) macro_command_test_command_new);

    const struct IView *view = puremvc_view_getInstance("ControllerTestKey1", puremvc_view_new);

    struct MacroCommandTestVO *vo = malloc(sizeof(struct MacroCommandTestVO));
    vo->input = 5;
    struct INotification *notification = puremvc_notification_new("MacroCommandTest", vo, NULL);

    view->notifyObservers(view, notification);

    // test assertions
    assert(vo->result1 == 10);
    assert(vo->result2 == 25);

    controller->removeCommand(controller, "MacroCommandTest");
    puremvc_notification_free(&notification);
    puremvc_controller_removeController("ControllerTest1");
    puremvc_view_removeView("ControllerTest1");
    controller = NULL;
}
