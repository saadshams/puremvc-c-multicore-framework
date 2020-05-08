#include "MacroCommandTest.h"
#include "MacroCommandTestVO.h"
#include "interfaces/Notification.h"
#include "interfaces/Notifier.h"
#include "MacroCommandTestCommand.h"
#include <assert.h>
#include <stdio.h>

int main() {
    testMacroCommandExecute();
    testMacroCommandExecute2();
    puts("MacroCommandTest: Success");
    return 0;
}

void testMacroCommandExecute() {
    MacroCommandTestVO *vo = MacroCommandTestVONew(5);
    Notification *notification = NewNotification("MacroCommandTest", vo, NULL);

    MacroCommand *command = MacroCommandTestCommandNew();
    command->notifier->initializeNotifier(command->notifier, "test");
    command->execute(command, notification);

    assert(vo->result1 == 10);
    assert(vo->result2 == 25);

    MacroCommandTestVODelete(vo);
    DeleteNotification(notification);
}

void testMacroCommandExecute2() {
    MacroCommandTestVO *vo = MacroCommandTestVONew(5);
    Notification *notification = NewNotification("MacroCommandTest", vo, NULL);

    MacroCommand *command = MacroCommandTestCommandNew2();
    command->notifier->initializeNotifier(command->notifier, "test");

    command->execute(command, notification);

    assert(vo->result1 == 10);
    assert(vo->result2 == 25);

    MacroCommandTestVODelete(vo);
    DeleteNotification(notification);
}