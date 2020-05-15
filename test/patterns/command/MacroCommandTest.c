#include "MacroCommandTest.h"
#include "MacroCommandTestVO.h"
#include "interfaces/Notification.h"
#include "interfaces/Notifier.h"
#include "MacroCommandTestCommand.h"
#include <assert.h>
#include <stdio.h>

int main() {
    testMacroCommandExecute();
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
    command->notifier->initializeNotifier(command->notifier, "test");

    // Execute the SimpleCommand
    command->execute(command, notification);

    // test assertions
    assert(vo.result1 == 10);
    assert(vo.result2 == 25);

    DeleteNotification(notification);
    DeleteMacroCommand(command);
}
