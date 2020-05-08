#include "SimpleCommandTest.h"
#include "SimpleCommandTestVO.h"
#include "interfaces/Notification.h"
#include "SimpleCommandTestCommand.h"
#include <assert.h>
#include <stdio.h>

int main() {
    testSimpleCommandExecute();
    puts("SimpleCommand: Success");
    return 0;
}

void testSimpleCommandExecute() {
    SimpleCommandTestVO *vo = SimpleCommandTestVONew(5);
    Notification *note = NewNotification("SimpleCommandTestNote", vo, NULL);

    SimpleCommand *command = NewSimpleCommandTestCommand();
    command->execute(command, note);
    assert(vo->result == 10);

    SimpleCommandTestVORelease(vo);
    DeleteNotification(note);
    DeleteSimpleCommand(command);
}