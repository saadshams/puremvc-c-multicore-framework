#pragma once

#include <stdbool.h>

#include "ICommand.h"
#include "IMediator.h"
#include "IProxy.h"
#include "INotification.h"

struct IFacade {
    void (*initializeFacade)(struct IFacade *self);
    void (*initializeController)(struct IFacade *self);
    void (*initializeModel)(struct IFacade *self);
    void (*initializeView)(struct IFacade *self);

    void (*registerCommand)(const struct IFacade *self, const char *notificationName, struct ICommand *(*factory)());
    bool (*hasCommand)(const struct IFacade *self, const char *notificationName);
    void (*removeCommand)(const struct IFacade *self, const char *notificationName);

    void (*registerProxy)(const struct IFacade *self, struct IProxy *proxy);
    struct IProxy *(*retrieveProxy)(const struct IFacade *self, const char *proxyName);
    bool (*hasProxy)(const struct IFacade *self, const char *proxyName);
    struct IProxy *(*removeProxy)(const struct IFacade *self, const char *proxyName);

    void (*registerMediator)(const struct IFacade *self, struct IMediator *mediator);
    struct IMediator *(*retrieveMediator)(const struct IFacade *self, const char *mediatorName);
    bool (*hasMediator)(const struct IFacade *self, const char *mediatorName);
    struct IMediator *(*removeMediator)(const struct IFacade *self, const char *mediatorName);

    void (*notifyObservers)(const struct IFacade *self, const struct INotification *notification);
    void (*sendNotification)(const struct IFacade *self, const char *notificationName, void *body, const char *type);
};

struct IFacade *puremvc_facade_new(const char *key);
void puremvc_facade_free(struct IFacade **facade);

struct IFacade *puremvc_facade_getInstance(const char *key, struct IFacade *(*factory)(const char *));
bool puremvc_facade_hasCore(const char *key);
void puremvc_facade_removeFacade(const char *key);
