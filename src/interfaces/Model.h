#ifndef PUREMVC_MODEL_H
#define PUREMVC_MODEL_H

#include "Proxy.h"
#include <stdbool.h>

/**
 * <P>A Multiton <code>Model</code> implementation.</P>
 *
 * <P>In PureMVC, the <code>Model</code> class provides
 * access to model objects (Proxies) by named lookup.</P>
 *
 * <P>The <code>Model</code> assumes these responsibilities:</P>
 *
 * <UL>
 * <LI>Maintain a cache of <code>Proxy</code> instances.</LI>
 * <LI>Provide methods for registering, retrieving, and removing
 * <code>Proxy</code> instances.</LI>
 * </UL>
 *
 * <P>Your application must register <code>Proxy</code> instances
 * with the <code>Model</code>. Typically, you use an
 * <code>Command</code> to create and register <code>Proxy</code>
 * instances once the <code>Facade</code> has initialized the Core
 * actors.</p>
 *
 * @see Proxy
 */
typedef struct ProxyMap ProxyMap;

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

/** Initializer */
void InitModel(Model *model);

void RemoveModel(const char *key);

#endif //PUREMVC_MODEL_H
