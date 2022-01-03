#include "SimpleCommandTest.h"
#include "SimpleCommandTestVO.h"
#include "interfaces/Notification.h"
#include "interfaces/Notifier.h"
#include "SimpleCommandTestCommand.h"
#include <assert.h>
#include <stdio.h>

/**
 * Test the PureMVC SimpleCommand class.
 *
 * @see SimpleCommandTestVO
 * @see SimpleCommandTestCommand
 */
int main(int argc, char *argv[]) {
    testSimpleCommandExecute();
    puts("SimpleCommand: Success");
    return 0;
}

/**
 * Tests the <code>execute</code> method of a <code>SimpleCommand</code>.
 *
 * <P>This test creates a new <code>Notification</code>, adding a
 * <code>SimpleCommandTestVO</code> as the body.
 * It then creates a <code>SimpleCommandTestCommand</code> and invokes
 * its <code>execute</code> method, passing in the note.</P>
 *
 * <P>Success is determined by evaluating a property on the
 * object that was passed on the Notification body, which will
 * be modified by the SimpleCommand</P>.
 *
 */
void testSimpleCommandExecute() {
    // Create the VO
    SimpleCommandTestVO vo = {5, 0};

    // Create the Notification (note)
    Notification *note = $Notification.new("SimpleCommandTestNote", &vo, NULL);

    // Create the SimpleCommand
    SimpleCommandTestCommand *command = NewSimpleCommandTestCommand();
    command->simpleCommand.notifier->initializeNotifier(command->simpleCommand.notifier, "test");

    // Execute the SimpleCommand
    command->simpleCommand.execute(&command->simpleCommand, note);

    // test assertions
    assert(vo.result == 10);

    $Notification.delete(note);
    DeleteSimpleCommandTestCommand(command);
    $Facade.removeFacade("test");
}