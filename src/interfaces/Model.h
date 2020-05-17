#ifndef PUREMVC_MODEL_H
#define PUREMVC_MODEL_H

#include "Proxy.h"
#include <stdbool.h>

/**
 * <P>A Multiton <code>IModel</code> implementation.</P>
 *
 * <P>In PureMVC, the <code>Model</code> class provides
 * access to model objects (Proxies) by named lookup.</P>
 *
 * <P>The <code>Model</code> assumes these responsibilities:</P>
 *
 * <UL>
 * <LI>Maintain a cache of <code>IProxy</code> instances.</LI>
 * <LI>Provide methods for registering, retrieving, and removing
 * <code>IProxy</code> instances.</LI>
 * </UL>
 *
 * <P>Your application must register <code>IProxy</code> instances
 * with the <code>Model</code>. Typically, you use an
 * <code>ICommand</code> to create and register <code>IProxy</code>
 * instances once the <code>Facade</code> has initialized the Core
 * actors.</p>
 *
 * @see org.puremvc.c.multicore.patterns.proxy.Proxy Proxy
 * @see org.puremvc.c.multicore.interfaces.IProxy Proxy
 */
typedef struct ProxyMap ProxyMap;

struct ProxyMap {
    const char *name;
    Proxy *proxy;
    ProxyMap *next;
};

typedef struct Model Model;

struct Model {
    const char *multitonKey;
    ProxyMap *proxyMap;
    void (*initializeModel)(Model *self);
    void (*registerProxy)(Model *self, Proxy *proxy);
    Proxy *(*retrieveProxy)(Model *self, const char *proxyName);
    bool (*hasProxy)(Model *self, const char *proxyName);
    Proxy *(*removeProxy)(Model *self, const char *proxyName);
};

Model *getModelInstance(const char *key, Model *(factory)(const char *));

Model *NewModel(const char *key);

/**
 * Initializer
 *
 * @param model
 */
void InitModel(Model *model);

void RemoveModel(const char *key);

#endif //PUREMVC_MODEL_H
