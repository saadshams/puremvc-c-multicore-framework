#ifndef PUREMVC_MODEL_H
#define PUREMVC_MODEL_H

#include "Proxy.h"
#include "Map.h"
#include <stdbool.h>

typedef struct Model Model;

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
struct Model {
    const char *multitonKey;

    Map *proxyMap;

    /**
     * <P>Initialize the <code>Model</code> instance.</P>
     *
     * <P>Called automatically by the constructor, this
     * is your opportunity to initialize the Singleton
     * instance in your subclass without overriding the
     * constructor.</P>
     *
     * @param self Model
     */
    void (*initializeModel)(Model *self);

    /**
     * <P>Register an <code>Proxy</code> with the <code>Model</code>.</P>
     *
     * @param self
     * @param proxy
     * @return Proxy* returns the old version for it to be released from memory if the value gets replaced
     */
    Proxy *(*registerProxy)(Model *self, Proxy *proxy);

    /**
     * <P>Retrieve an <code>Proxy</code> from the <code>Model</code>.</P>
     *
     * @param self Model
     * @param proxyName new name
     * @return the <code>Proxy</code> instance previously registered with the given <code>proxyName</code>.
     */
    Proxy *(*retrieveProxy)(Model *self, const char *proxyName);

    /**
     * <P>Check if a Proxy is registered</P>
     *
     * @param self Model
     * @param proxyName new name
     * @return whether a Proxy is currently registered with the given <code>proxyName</code>.
     */
    bool (*hasProxy)(Model *self, const char *proxyName);

    /**
     * <P>Remove an <code>Proxy</code> from the <code>Model</code>.</P>
     *
     * @param self Model
     * @param proxyName name of the <code>Proxy</code> instance to be removed.
     * @return the <code>Proxy</code> that was removed from the <code>Model</code>
     */
    Proxy *(*removeProxy)(Model *self, const char *proxyName);
};

struct $Model {
    /**
     * Constructor.
     *
     * <P>
     * This <code>IModel</code> implementation is a Multiton,
     * so you should not call the constructor
     * directly, but instead call the static Multiton
     * Factory method <code>Model.getInstance( multitonKey )</code>
     *
     * @throws Error Error if instance for this Multiton key instance has already been constructed
     *
     */
    Model *(*new)(const char *key);

    /** Initializer */
    void (*init)(Model *model);

    /** Remove Core */
    void (*removeModel)(const char *key);

    /**
     * <P><code>Model</code> Multiton Factory method.</P>
     *
     * @param key multitonKey
     * @param factory factory that returns <code>Model</code>
     * @return the Multiton instance of <code>Model</code>
     */
    Model *(*getInstance)(const char *key, Model *(factory)(const char *));
};

const struct $Model $Model;

// ModelNode LinkedList
//typedef struct ModelNode ModelNode;
//
//struct ModelNode {
//    const char *name;
//    Model *model;
//    ModelNode *next;
//};
//
//struct $ModelNode {
//    ModelNode *(*get)(ModelNode *head, const char *key);
//
//    ModelNode *(*put)(ModelNode **head, const char *key, Model *model);
//
//    void (*remove)(ModelNode **head, const char *key);
//};
//
//const struct $ModelNode $ModelNode;

#endif //PUREMVC_MODEL_H
