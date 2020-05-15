#ifndef PUREMVC_SIMPLECOMMAND_H
#define PUREMVC_SIMPLECOMMAND_H

#include "Notification.h"

struct Notifier;

typedef struct SimpleCommand SimpleCommand;

struct SimpleCommand {
    struct Notifier *notifier;
    void (*execute)(SimpleCommand *self, Notification *notification);
};

void InitSimpleCommand(SimpleCommand *self);

SimpleCommand *NewSimpleCommand();

void DeleteSimpleCommand(SimpleCommand *self);

#endif //PUREMVC_SIMPLECOMMAND_H
