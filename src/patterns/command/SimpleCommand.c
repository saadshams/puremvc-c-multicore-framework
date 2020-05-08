#include "interfaces/SimpleCommand.h"
#include "interfaces/Notifier.h"
#include <stdlib.h>

static void execute(const SimpleCommand *self, Notification *notification) {

}

void InitSimpleCommand(SimpleCommand *self) {
    if (self) {
        self->execute = execute;
        self->notifier = NewNotifier();
    }
}

SimpleCommand *NewSimpleCommand() {
    SimpleCommand *self = malloc(sizeof(SimpleCommand));
    InitSimpleCommand(self);
    return self;
}

void DeleteSimpleCommand(SimpleCommand *self) {
    free(self);
}
