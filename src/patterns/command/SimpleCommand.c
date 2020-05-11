#include "interfaces/SimpleCommand.h"
#include "interfaces/Notifier.h"
#include <stdio.h>
#include <stdlib.h>

static void execute(const SimpleCommand *self, Notification *notification) {

}

void InitSimpleCommand(SimpleCommand *self) {
    self->notifier = NewNotifier();
    self->execute = execute;
}

SimpleCommand *NewSimpleCommand() {
    SimpleCommand *self = malloc(sizeof(SimpleCommand));
    if (self == NULL) goto exception;
    InitSimpleCommand(self);
    return self;

    exception:
        fprintf(stderr, "SimpleCommand allocation failed.\n");
        return NULL;
}

void DeleteSimpleCommand(SimpleCommand *self) {
    DeleteNotifier(self->notifier);
    free(self);
    self = NULL;
}
