#ifndef PUREMVC_PROXY_H
#define PUREMVC_PROXY_H

#include <stdlib.h>

struct Notifier;

/**
 * <P>A base <code>IProxy</code> implementation.</P>
 *
 * <P>In PureMVC, <code>Proxy</code> classes are used to manage parts of the
 * application's data model.</P>
 *
 * <P>A <code>Proxy</code> might simply manage a reference to a local data object,
 * in which case interacting with it might involve setting and
 * getting of its data in synchronous fashion.</P>
 *
 * <P><code>Proxy</code> classes are also used to encapsulate the application's
 * interaction with remote services to save or retrieve data, in which case,
 * we adopt an asynchronous idiom; setting data (or calling a method) on the
 * <code>Proxy</code> and listening for a <code>Notification</code> to be sent
 * when the <code>Proxy</code> has retrieved the data from the service.</P>
 *
 * @see org.puremvc.c.multicore.core.Model Model
 */
typedef struct Proxy Proxy;

struct Proxy {
    struct Notifier *notifier;
    const char *proxyName;
    void *data;
    const char *(*getProxyName)(Proxy *self);
    void (*setData)(Proxy *self, void *data);
    void *(*getData)(Proxy *self);
    void (*onRegister)(Proxy *self);
    void (*onRemove)(Proxy *self);
};

/**
 * <P>The name of the <code>Proxy</code>.</P>
 */
#define PROXY_NAME "Proxy"

/**
 * Initializer
 *
 * @param self
 * @param proxyName
 * @param data
 */
void InitProxy(Proxy *self, const char *proxyName, void *data);

/**
 * Constructor
 *
 * @param proxyName
 * @param data
 */
Proxy *NewProxy(const char *proxyName, void *data);

/**
 * Destructor
 *
 * @param self
 */
void DeleteProxy(Proxy *self);

#endif //PUREMVC_PROXY_H
