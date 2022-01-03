#include "interfaces/SimpleCommand.h"
#include "interfaces/Notifier.h"
#include <stdio.h>
#include <stdlib.h>

static void execute(SimpleCommand *self, Notification *notification) {

}

static void init(SimpleCommand *simpleCommand) {
    simpleCommand->notifier = $Notifier.new();
    simpleCommand->execute = execute;
}

static SimpleCommand *new(void) {
    SimpleCommand *simpleCommand = malloc(sizeof(SimpleCommand));
    if (simpleCommand == NULL) goto exception;
    $SimpleCommand.init(simpleCommand);
    return simpleCommand;

    exception:
    fprintf(stderr, "SimpleCommand allocation failed.\n");
    return NULL;
}

static void delete(SimpleCommand *simpleCommand) {
    $Notifier.delete(simpleCommand->notifier);
    free(simpleCommand);
    simpleCommand = NULL;
}

const struct $SimpleCommand $SimpleCommand = {.new = new, .init = init, .delete = delete};