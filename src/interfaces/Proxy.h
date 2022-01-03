#ifndef PUREMVC_PROXY_H
#define PUREMVC_PROXY_H

#include <stdlib.h>
#include <stdbool.h>

/** The name of the <code>Proxy</code>.*/
#define PROXY_NAME "Proxy"

typedef struct Proxy Proxy;

struct Notifier;

/**
 * <P>A base <code>Proxy*</code> implementation.</P>
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
 * @see Model
 */
struct Proxy {

    struct Notifier *notifier;

    const char *proxyName;

    void *data;

    /** Get the new name */
    const char *(*getProxyName)(Proxy *self);

    /** Set the data object */
    void (*setData)(Proxy *self, void *data);

    /** Get the data object */
    void *(*getData)(Proxy *self);

    /** Called by the Model when the Proxy is registered */
    void (*onRegister)(Proxy *self);

    /** Called by the Model when the Proxy is removed */
    void (*onRemove)(Proxy *self);
};

struct $Proxy {
    /** Constructor */
    Proxy *(*new)(const char *proxyName, void *data);

    /** Initializer */
    void (*init)(Proxy *proxy, const char *proxyName, void *data);

    /** Destructor */
    void (*delete)(Proxy *proxy);
};

const struct $Proxy $Proxy;

#endif //PUREMVC_PROXY_H
