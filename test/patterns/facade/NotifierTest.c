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

void testInstance() {
    Notifier *notifier = NewNotifier();
    notifier->initializeNotifier(notifier, "Test1");
    assert(notifier != NULL);
    assert(notifier->getFacade(notifier) != NULL);
    DeleteNotifier(notifier);
}

typedef struct {
    int value;
    int result;
} Object;

static void execute(const SimpleCommand *self, Notification *notification) {
    Object *temp = notification->getBody(notification);
    temp->result = temp->value * 4;
}

static SimpleCommand *NewCommand() {
    SimpleCommand *command = NewSimpleCommand();
    command->execute = execute;
    return command;
}

void testRegisterCommandAndSendNotification() {
    Notifier *notifier = NewNotifier();
    notifier->initializeNotifier(notifier, "Test2");
    Object temp = {4};

    Facade *facade = notifier->getFacade(notifier);
    facade->registerCommand(facade, "TestNote", NewCommand);
    notifier->sendNotification(notifier, "TestNote", &temp, NULL);

    assert(temp.result == 16);
    DeleteNotifier(notifier);
}
