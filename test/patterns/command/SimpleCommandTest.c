#include <assert.h>
#include <stdlib.h>

#include "SimpleCommandTest.h"
#include "SimpleCommandTestCommand.h"
#include "SimpleCommandTestVO.h"

int main() {
    testConstructor();
    testSimpleCommandExecute();
    return 0;
}

/**
 * Test Constructor
 */
void testConstructor() {
    struct ICommand *command = puremvc_simple_command_new();

    assert(command != NULL);
    puremvc_simple_command_free(&command);
    assert(command == NULL);
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
 */
void testSimpleCommandExecute() {
    // Create the VO
    struct SimpleCommandTestVO *vo = malloc(sizeof(struct SimpleCommandTestVO));
    if (vo) vo->input = 5;

    // Create the Notification (note)
    struct INotification *note = puremvc_notification_new("SimpleCommandTestNote", vo, NULL);

    // Create the SimpleCommand
    struct ICommand *command = test_simple_command_new();

    // Execute the SimpleCommand
    command->execute(command, note);

    // test assertions
    assert(vo->result == 10);

    puremvc_notification_free(&note);
    assert(note == NULL);
    puremvc_simple_command_free(&command);
    assert(command == NULL);
}
