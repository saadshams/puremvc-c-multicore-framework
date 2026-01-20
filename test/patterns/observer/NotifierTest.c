#include <assert.h>
#include <stdio.h>

#include "puremvc/puremvc.h"
#include "NotifierTest.h"

int main() {
    testInstance();
    // testRegisterCommandAndSendNotification();
    return 0;
}

struct Object {
    int value;
    int result;
};

static void execute(const struct ICommand *self, struct INotification *notification) {
    struct Object *temp = (struct Object *)notification->getBody(notification);

    // fabricate a result
    temp->result = temp->value * 4;
}

static struct ICommand *command_new() {
    struct ICommand *command = puremvc_simple_command_new();
    command->execute = execute;
    return command;
}

void testInstance() {
    // create notifier instance
    struct INotifier *notifier = puremvc_notifier_new();

    // initialize facade
    notifier->initializeNotifier(notifier, "NotifierTest1");

    // assert
    // assert(notifier != NULL);
    // assert(notifier->getFacade(notifier) != NULL);

    puremvc_facade_removeFacade("NotifierTest1");
    puremvc_notifier_free(&notifier);
}

void testRegisterCommandAndSendNotification() {
    // create a notifier
    struct INotifier *notifier = puremvc_notifier_new();

    // initialize facade
    notifier->initializeNotifier(notifier, "NotifierTest2");

    struct Object temp = {4};

    // get facade instance
    const struct IFacade *facade = notifier->getFacade(notifier);

    // register a command and send notification
    facade->registerCommand(facade, "TestNote", command_new);
    notifier->sendNotification(notifier, "TestNote", &temp, NULL);

    // assert result
    assert(temp.result == 16);

    facade->removeCommand(facade, "TestNote");
    puremvc_facade_removeFacade("NotifierTest2");
    puremvc_notifier_free(&notifier);
}
