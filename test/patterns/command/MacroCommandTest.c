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

    const char *error = NULL;
    struct INotification *notification = puremvc_notification_new("MacroCommandTest", vo, NULL, &error);

    struct IMacroCommand *macroCommand = macro_command_test_command_new(&error);
    assert(macroCommand != NULL);
    assert(macroCommand->base.notifier != NULL);

    macroCommand->base.notifier->initializeNotifier(macroCommand->base.notifier, "MacroCommandTestkey1", &error);
    macroCommand->base.execute(&macroCommand->base, notification, &error);

    assert(vo->result1 == 10);
    assert(vo->result2 == 25);
    assert(vo->result3 == 125);

    puremvc_macro_command_free(&macroCommand);
}

void testRegisterAndExecuteCommand() {
    const char *error = NULL;
    const struct IController *controller = puremvc_controller_getInstance("ControllerTestKey1", puremvc_controller_new, &error);
    assert(error == NULL);

    controller->registerCommand(controller, "MacroCommandTest", (struct ICommand *(*)(const char **)) macro_command_test_command_new, &error);
    assert(error == NULL);

    const struct IView *view = puremvc_view_getInstance("ControllerTestKey1", puremvc_view_new, &error);
    assert(error == NULL);

    struct MacroCommandTestVO *vo = malloc(sizeof(struct MacroCommandTestVO));
    vo->input = 5;
    struct INotification *notification = puremvc_notification_new("MacroCommandTest", vo, NULL, &error);
    assert(error == NULL);

    view->notifyObservers(view, notification, &error);

    // test assertions
    assert(vo->result1 == 10);
    assert(vo->result2 == 25);
    assert(vo->result3 == 125);

    controller->removeCommand(controller, "MacroCommandTest");
    puremvc_notification_free(&notification);
    puremvc_controller_removeController("ControllerTest1");
    puremvc_view_removeView("ControllerTest1");
    controller = NULL;
}
