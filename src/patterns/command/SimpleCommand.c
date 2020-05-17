#include "interfaces/SimpleCommand.h"
#include "interfaces/Notifier.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * <P>Fulfill the use-case initiated by the given <code>INotification</code>.</P>
 *
 * <P>In the Command Pattern, an application use-case typically
 * begins with some user action, which results in an <code>INotification</code> being broadcast, which
 * is handled by business logic in the <code>execute</code> method of an
 * <code>ICommand</code>.</P>
 *
 * @param self
 * @param notification the <code>INotification</code> to handle.
 */
static void execute(SimpleCommand *self, Notification *notification) {

}

/**
 * Initializer
 *
 * @param simpleCommand
 */
void InitSimpleCommand(SimpleCommand *simpleCommand) {
    simpleCommand->notifier = NewNotifier();
    simpleCommand->execute = execute;
}

/**
 * Constructor
 */
SimpleCommand *NewSimpleCommand() {
    SimpleCommand *simpleCommand = malloc(sizeof(SimpleCommand));
    if (simpleCommand == NULL) goto exception;
    InitSimpleCommand(simpleCommand);
    return simpleCommand;

    exception:
        fprintf(stderr, "SimpleCommand allocation failed.\n");
        return NULL;
}

/**
 * Destructor
 *
 * @param simpleCommand
 */
void DeleteSimpleCommand(SimpleCommand *simpleCommand) {
    DeleteNotifier(simpleCommand->notifier);
    free(simpleCommand);
    simpleCommand = NULL;
}
