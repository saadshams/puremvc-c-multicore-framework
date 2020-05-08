#include "interfaces/SimpleCommand.h"
#include "interfaces/Notifier.h"
#include <stdlib.h>

static void execute(const SimpleCommand *self, Notification *notification) {

}

void InitSimpleCommand(SimpleCommand *self) {
    if (self) {
        self->notifier = NewNotifier();
        self->execute = execute;
    }
}

SimpleCommand *NewSimpleCommand() {
    SimpleCommand *self = malloc(sizeof(SimpleCommand));
    InitSimpleCommand(self);
    return self;
}

void DeleteSimpleCommand(SimpleCommand *self) {
    DeleteNotifier(self->notifier);
    free(self);
}
