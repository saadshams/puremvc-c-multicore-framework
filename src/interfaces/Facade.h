#ifndef PUREMVC_FACADE_H
#define PUREMVC_FACADE_H

#include "Controller.h"
#include "Model.h"
#include "View.h"

typedef struct Facade Facade;

struct Facade {
    const char *multitonKey;
    Controller *controller;
    Model *model;
    View *view;
    void (*initializeFacade)(Facade *self);
    void (*initializeController)(Facade *self);
    void (*initializeModel)(Facade *self);
    void (*initializeView)(Facade *self);
    void (*registerCommand)(Facade *self, const char *notificationName, SimpleCommand *(*factory)());
    void (*removeCommand)(Facade *self, const char *notificationName);
    bool (*hasCommand)(Facade *self, const char *notificationName);
    void (*registerProxy)(Facade *self, Proxy *proxy);
    Proxy *(*retrieveProxy)(Facade *self, const char *proxyName);
    Proxy *(*removeProxy)(Facade *self, const char *proxyName);
    bool (*hasProxy)(Facade *self, const char *proxyName);
    void (*registerMediator)(Facade *self, Mediator *mediator);
    Mediator *(*retrieveMediator)(Facade *self, const char *mediatorName);
    Mediator *(*removeMediator)(Facade *self, const char *mediatorName);
    bool (*hasMediator)(Facade *self, const char *mediatorName);
    void (*sendNotification)(Facade *self, const char *notificationName, void *body, char *type);
    void (*notifyObservers)(Facade *self, Notification *notification);
    void (*initializeNotifier)(Facade *self, const char *key);
};

void InitFacade(Facade *self);

Facade *NewFacade(const char *key);

bool HasFacadeCore(const char *key);

void RemoveFacadeCore(const char *key);

Facade *getFacadeInstance(const char *key, Facade *(*factory)(const char *));

#endif //PUREMVC_FACADE_H
