#ifndef PUREMVC_FACADE_H
#define PUREMVC_FACADE_H

#include "Controller.h"
#include "Model.h"
#include "View.h"

/**
 * A base Multiton <code>IFacade</code> implementation.
 *
 * @see org.puremvc.c.multicore.core.Model Model
 * @see org.puremvc.c.multicore.core.View View
 * @see org.puremvc.c.multicore.core.Controller Controller
 */
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

/**
 * Facade Multiton Factory method
 *
 * @param key
 * @param factory
 * @return facade
 */
Facade *getFacadeInstance(const char *key, Facade *(*factory)(const char *));

/**
 * Constructor
 *
 * @param key
 */
Facade *NewFacade(const char *key);

/**
 * Initializer
 *
 * @param facade
 */
void InitFacade(Facade *facade);

/**
 * Check if a Core is registered or not
 *
 * @param key
 * @return bool
 */
bool HasFacadeCore(const char *key);

/**
 * Remove a Core.
 *
 * @param key
 */
void RemoveFacade(const char *key);

#endif //PUREMVC_FACADE_H
