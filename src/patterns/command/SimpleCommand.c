#include "interfaces/SimpleCommand.h"
#include "interfaces/Notifier.h"
#include <stdio.h>
#include <stdlib.h>

static void execute(SimpleCommand *self, Notification *notification) {

}

SimpleCommand *NewSimpleCommand() {
    SimpleCommand *simpleCommand = malloc(sizeof(SimpleCommand));
    if (simpleCommand == NULL) goto exception;
    InitSimpleCommand(simpleCommand);
    return simpleCommand;

    exception:
        fprintf(stderr, "SimpleCommand allocation failed.\n");
        return NULL;
}

void InitSimpleCommand(SimpleCommand *simpleCommand) {
    simpleCommand->notifier = NewNotifier();
    simpleCommand->execute = execute;
}

void DeleteSimpleCommand(SimpleCommand *simpleCommand) {
    DeleteNotifier(simpleCommand->notifier);
    free(simpleCommand);
    simpleCommand = NULL;
}
