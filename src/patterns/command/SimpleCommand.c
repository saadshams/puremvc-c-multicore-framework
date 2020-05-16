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
 * @param self
 */
void InitSimpleCommand(SimpleCommand *self) {
    self->notifier = NewNotifier();
    self->execute = execute;
}

/**
 * Constructor
 */
SimpleCommand *NewSimpleCommand() {
    SimpleCommand *self = malloc(sizeof(SimpleCommand));
    if (self == NULL) goto exception;
    InitSimpleCommand(self);
    return self;

    exception:
        fprintf(stderr, "SimpleCommand allocation failed.\n");
        return NULL;
}

/**
 * Destructor
 *
 * @param self
 */
void DeleteSimpleCommand(SimpleCommand *self) {
    DeleteNotifier(self->notifier);
    free(self);
    self = NULL;
}
