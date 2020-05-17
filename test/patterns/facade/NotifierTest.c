#include "NotifierTest.h"
#include "interfaces/Notifier.h"
#include <stdio.h>
#include <assert.h>

int main() {
    testInstance();
    testRegisterCommandAndSendNotification();
    puts("NotifierTest: Success");
    return 0;
}

typedef struct {
    int value;
    int result;
} Object;

/**
 * test Notifier
 */
void testInstance() {
    // create notifier instance
    Notifier *notifier = NewNotifier();

    // initialize facade
    notifier->initializeNotifier(notifier, "Test1");

    // assert
    assert(notifier != NULL);
    assert(notifier->getFacade(notifier) != NULL);

    RemoveFacade("Test1");
    DeleteNotifier(notifier);
}

/**
 * an implementation that multiplies the input value by 4 and stores into result
 */
static void execute(SimpleCommand *self, Notification *notification) {
    Object *temp = notification->getBody(notification);

    // fabricate a result
    temp->result = temp->value * 4;
}

/**
 * Create a simple command
 */
static SimpleCommand *NewCommand() {
    SimpleCommand *command = NewSimpleCommand();
    command->execute = execute;
    return command;
}

/**
 * Register a command and test by sending a notification
 */
void testRegisterCommandAndSendNotification() {
    // create a notifier
    Notifier *notifier = NewNotifier();

    // initialize facade
    notifier->initializeNotifier(notifier, "Test2");

    Object temp = {4};

    // get facade instance
    Facade *facade = notifier->getFacade(notifier);

    // register a command and send notification
    facade->registerCommand(facade, "TestNote", NewCommand);
    notifier->sendNotification(notifier, "TestNote", &temp, NULL);

    // assert result
    assert(temp.result == 16);

    facade->removeCommand(facade, "TestNote");
    RemoveFacade("Test2");
    DeleteNotifier(notifier);
}
