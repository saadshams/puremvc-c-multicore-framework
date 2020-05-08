#ifndef PUREMVC_CONTROLLER_H
#define PUREMVC_CONTROLLER_H

#include "SimpleCommand.h"
#include "View.h"
#include <stdbool.h>

typedef struct CommandMap CommandMap;

struct CommandMap {
    char *name;
    SimpleCommand *(*factory)();
    CommandMap *next;
};

typedef struct Controller Controller;

struct Controller {
    char *multitonKey;
    View *view;
    CommandMap *commandMap;
    void (*initializeController)(Controller *self);
    void (*executeCommand)(Controller *self, Notification *notification);
    void (*registerCommand)(Controller *self, const char *notificationName, SimpleCommand *(*factory)());
    bool (*hasCommand)(Controller *self,  const char *notificationName);
    void (*removeCommand)(Controller *self, const char *notificationName);
};

void InitController(Controller *self);

Controller *NewController(char *key);

void DeleteController(char *key);

Controller *getControllerInstance(char *key, Controller *(*factory)(char *));

#endif //PUREMVC_CONTROLLER_H
