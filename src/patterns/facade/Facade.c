#include "interfaces/Facade.h"
#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>

// FacadeMap LinkedList
typedef struct FacadeMap FacadeMap;

struct FacadeMap {
    const char *name;
    Facade *facade;
    FacadeMap *next;
};

// The Multiton Facade instanceMap.
static FacadeMap *instanceMap;

// mutex for instanceMap
static pthread_rwlock_t facade_mutex = PTHREAD_MUTEX_INITIALIZER;

/**
 * Construct a new instanceMap node
 *
 * @param key
 * @param facade
 * @return facadeMap
 */
static FacadeMap *NewFacadeMap(const char *key, Facade *facade) {
    FacadeMap *facadeMap = malloc(sizeof(FacadeMap));
    if (facadeMap == NULL) goto exception;
    facadeMap->name = key;
    facadeMap->facade = facade;
    facadeMap->next = NULL;
    return facadeMap;

    exception:
        fprintf(stderr, "FacadeMap allocation failed.\n");
        return NULL;
}

/**
 * Retrieve a Node from instanceMap LinkedList
 *
 * @param key
 * @return facade
 */
static Facade *GetFacadeMap(const char *key) {
    FacadeMap *cursor = instanceMap;
    while (cursor && strcmp(cursor->name, key) != 0)
        cursor = cursor->next;
    return cursor == NULL ? NULL : cursor->facade;
}

/**
 * Add a Node to the instanceMap LinkedList
 *
 * @param key
 * @param facade
 */
static void AddFacadeMap(const char *key, Facade *facade) {
    FacadeMap **cursor = &instanceMap;
    while (*cursor)
        cursor = &(*cursor)->next;
    *cursor = NewFacadeMap(key, facade);
}

/**
 * Remove a node from instanceMap LinkedList
 *
 * @param key
 */
static void RemoveFacadeMap(const char *key) {
    FacadeMap **cursor = &instanceMap;
    while (*cursor) {
        if (strcmp((*cursor)->name, key) == 0) {
            FacadeMap *node = *cursor;
            *cursor = (*cursor)->next;
            free(node->facade);
            free(node);
            node = NULL;
            break;
        }
        cursor = &(*cursor)->next;
    }
}

/**
 * <P>Initialize the Multiton <code>Facade</code> instance.</P>
 *
 * <P>Called automatically by the constructor. Override in your
 * subclass to do any subclass specific initializations. Be
 * sure to call <code>super.initializeFacade()</code>, though.</P>
 *
 * @param Facade*
 */
static void initializeFacade(Facade *self) {
    self->initializeModel(self);
    self->initializeController(self);
    self->initializeView(self);
}

/**
 * <P>Initialize the <code>Controller</code>.</P>
 *
 * <P>Called by the <code>initializeFacade</code> method.
 * Override this method in your subclass of <code>Facade</code>
 * if one or both of the following are true:</P>
 *
 * <UL>
 * <LI> You wish to initialize a different <code>IController</code>.</LI>
 * <LI> You have <code>Commands</code> to register with the <code>Controller</code> at startup.. </LI>
 * </UL>
 *
 * <P>If you don't want to initialize a different <code>IController</code>,
 * call <code>super.initializeController()</code> at the beginning of your
 * method, then register <code>Command</code>s.</P>
 *
 * @param self
 */
static void initializeController(Facade *self) {
    if (self->controller != NULL) return;
    self->controller = getControllerInstance(self->multitonKey, NewController);
}

/**
 * <P>Initialize the <code>Model</code>.</P>
 *
 * <P>Called by the <code>initializeFacade</code> method.
 * Override this method in your subclass of <code>Facade</code>
 * if one or both of the following are true:</P>
 *
 * <UL>
 * <LI> You wish to initialize a different <code>IModel</code>.</LI>
 * <LI> You have <code>Proxy</code>s to register with the Model that do not
 * retrieve a reference to the Facade at construction time.</LI>
 * </UL>
 *
 * <P>If you don't want to initialize a different <code>IModel</code>,
 * call <code>super.initializeModel()</code> at the beginning of your
 * method, then register <code>Proxy</code>s.</P>
 *
 * <P>Note: This method is <i>rarely</i> overridden; in practice you are more
 * likely to use a <code>Command</code> to create and register <code>Proxy</code>s
 * with the <code>Model</code>, since <code>Proxy</code>s with mutable data will likely
 * need to send <code>INotification</code>s and thus will likely want to fetch a reference to
 * the <code>Facade</code> during their construction.</P>
 *
 * @param self
 */
static void initializeModel(Facade *self) {
    if (self->model != NULL) return;
    self->model = getModelInstance(self->multitonKey, NewModel);
}

/**
 * <P>Initialize the <code>View</code>.</P>
 *
 * <P>Called by the <code>initializeFacade</code> method.
 * Override this method in your subclass of <code>Facade</code>
 * if one or both of the following are true:</P>
 *
 * <UL>
 * <LI> You wish to initialize a different <code>IView</code>.</LI>
 * <LI> You have <code>Observers</code> to register with the <code>View</code></LI>
 * </UL>
 *
 * <P>If you don't want to initialize a different <code>IView</code>,
 * call <code>super.initializeView()</code> at the beginning of your
 * method, then register <code>IMediator</code> instances.</P>
 *
 * <P>Note: This method is <i>rarely</i> overridden; in practice you are more
 * likely to use a <code>Command</code> to create and register <code>Mediator</code>s
 * with the <code>View</code>, since <code>IMediator</code> instances will need to send
 * <code>INotification</code>s and thus will likely want to fetch a reference
 * to the <code>Facade</code> during their construction.</P>
 *
 * @param self
 */
static void initializeView(Facade *self) {
    if (self->view != NULL) return;
    self->view = getViewInstance(self->multitonKey, NewView);
}

/**
 * <P>Register an <code>ICommand</code> with the <code>Controller</code> by Notification name.</P>
 *
 * @param self
 * @param notificationName the name of the <code>INotification</code> to associate the <code>ICommand</code> with
 * @param factory factory that returns <code>IController</code>
 */
static void registerCommand(Facade *self, const char *notificationName, SimpleCommand *(*factory)()) {
    self->controller->registerCommand(self->controller, notificationName, factory);
}

/**
 * <P>Remove a previously registered <code>ICommand</code> to <code>INotification</code> mapping from the Controller.</P>
 *
 * @param self
 * @param notificationName the name of the <code>INotification</code> to remove the <code>ICommand</code> mapping for
 */
static void removeCommand(Facade *self, const char *notificationName) {
    self->controller->removeCommand(self->controller, notificationName);
}

/**
 * <P>Check if a Command is registered for a given Notification</P>
 *
 * @param self
 * @param notificationName notification name
 * @return whether a Command is currently registered for the given <code>notificationName</code>.
 */
static bool hasCommand(Facade *self, const char *notificationName) {
    return self->controller->hasCommand(self->controller, notificationName);
}

/**
 * <P>Register an <code>IProxy</code> with the <code>Model</code> by name.</P>
 *
 * @param self
 * @param proxy the <code>IProxy</code> instance to be registered with the <code>Model</code>.
 */
static void registerProxy(Facade *self, Proxy *proxy) {
    self->model->registerProxy(self->model, proxy);
}

/**
 * <P>Retrieve an <code>IProxy</code> from the <code>Model</code> by name.</P>
 *
 * @param self
 * @param proxyName the name of the proxy to be retrieved.
 * @return the <code>IProxy</code> instance previously registered with the given <code>proxyName</code>.
 */
static Proxy *retrieveProxy(Facade *self, const char *proxyName) {
    return self->model->retrieveProxy(self->model, proxyName);
}

/**
 * <P>Remove an <code>IProxy</code> from the <code>Model</code> by name.</P>
 *
 * @param self
 * @param proxyName the <code>IProxy</code> to remove from the <code>Model</code>.
 * @return the <code>IProxy</code> that was removed from the <code>Model</code>
 */
static Proxy *removeProxy(Facade *self, const char *proxyName) {
    return self->model->removeProxy(self->model, proxyName);
}

/**
 * <P>Check if a Proxy is registered</P>
 *
 * @param self
 * @param proxyName proxy name
 * @return whether a Proxy is currently registered with the given <code>proxyName</code>.
 */
static bool hasProxy(Facade *self, const char *proxyName) {
    return self->model->hasProxy(self->model, proxyName);
}

/**
 * <P>Register a <code>IMediator</code> with the <code>View</code>.</P>
 *
 * @param self
 * @param mediator a reference to the <code>IMediator</code>
 */
static void registerMediator(Facade *self, Mediator *mediator) {
    self->view->registerMediator(self->view, mediator);
}

/**
 * <P>Retrieve an <code>IMediator</code> from the <code>View</code>.</P>
 *
 * @param self
 * @param mediatorName mediator name
 * @return the <code>IMediator</code> previously registered with the given <code>mediatorName</code>.
 */
static Mediator *retrieveMediator(Facade *self, const char *mediatorName) {
    return self->view->retrieveMediator(self->view, mediatorName);
}

/**
 * <P>Remove an <code>IMediator</code> from the <code>View</code>.</P>
 *
 * @param self
 * @param mediatorName name of the <code>IMediator</code> to be removed.
 * @return the <code>IMediator</code> that was removed from the <code>View</code>
 */
static Mediator *removeMediator(Facade *self, const char *mediatorName) {
    return self->view->removeMediator(self->view, mediatorName);
}

/**
 * <P>Check if a Mediator is registered or not</P>
 *
 * @param self
 * @param mediatorName mediator name
 * @return whether a Mediator is registered with the given <code>mediatorName</code>.
 */
static bool hasMediator(Facade *self, const char *mediatorName) {
    return self->view->hasMediator(self->view, mediatorName);
}

/**
 * <P>Create and send an <code>INotification</code>.</P>
 *
 * <P>Keeps us from having to construct new notification
 * instances in our implementation code.</P>
 *
 * @param self
 * @param notificationName the name of the notiification to send
 * @param body the body of the notification
 * @param type the type of the notification
 */
static void sendNotification(Facade *self, const char *notificationName, void *body, char *type) {
    Notification *notification = NewNotification(notificationName, body, type);
    self->notifyObservers(self, notification);
    DeleteNotification(notification);
}

/**
 * <P>Notify <code>Observer</code>s.</P>
 *
 * <P>This method is left public mostly for backward
 * compatibility, and to allow you to send custom
 * notification classes using the facade.</P>
 *
 * <P>Usually you should just call sendNotification
 * and pass the parameters, never having to
 * construct the notification yourself.</P>
 *
 * @param self
 * @param notification the <code>INotification</code> to have the <code>View</code> notify <code>Observers</code> of.
 */
static void notifyObservers(Facade *self, Notification *notification) {
    self->view->notifyObservers(self->view, notification);
}

/**
 * <P>Set the Multiton key for this facade instance.</P>
 *
 * <P>Not called directly, but instead from the
 * constructor when getInstance is invoked.
 * It is necessary to be public in order to
 * implement INotifier.</P>
 *
 * @param self
 */
static void initializeNotifier(Facade *self, const char *key) {
    self->multitonKey = key;
}

/**
 * Initializer
 *
 * @param facade
 */
void InitFacade(Facade *facade) {
    facade->controller = NULL;
    facade->model = NULL;
    facade->view = NULL;
    facade->initializeFacade = initializeFacade;
    facade->initializeController = initializeController;
    facade->initializeModel = initializeModel;
    facade->initializeView = initializeView;
    facade->registerCommand = registerCommand;
    facade->removeCommand = removeCommand;
    facade->hasCommand = hasCommand;
    facade->registerProxy = registerProxy;
    facade->retrieveProxy = retrieveProxy;
    facade->removeProxy = removeProxy;
    facade->hasProxy = hasProxy;
    facade->registerMediator = registerMediator;
    facade->retrieveMediator = retrieveMediator;
    facade->removeMediator = removeMediator;
    facade->hasMediator = hasMediator;
    facade->sendNotification = sendNotification;
    facade->notifyObservers = notifyObservers;
    facade->initializeNotifier = initializeNotifier;
}

/**
 * Constructor
 *
 * @param key
 */
Facade *NewFacade(const char *key) {
    assert(GetFacadeMap(key) == NULL);
    Facade *facade = malloc(sizeof(Facade));
    if (facade == NULL) goto exception;
    InitFacade(facade);
    facade->multitonKey = key;
    return facade;

    exception:
        fprintf(stderr, "Facade allocation failed.\n");
        return NULL;
}

/**
 * Check if a Core is registered or not
 *
 * @param key
 * @return bool
 */
bool HasFacadeCore(const char *key) {
    pthread_rwlock_rdlock(&facade_mutex);
    bool result = GetFacadeMap(key) != NULL;
    pthread_rwlock_unlock(&facade_mutex);
    return result;
}

/**
 * Remove a Core.
 *
 * @param key
 */
void RemoveFacade(const char *key) {
    pthread_rwlock_wrlock(&facade_mutex);
    RemoveModel(key);
    RemoveView(key);
    RemoveController(key);
    RemoveFacadeMap(key);
    pthread_rwlock_unlock(&facade_mutex);
}

/**
 * Facade Multiton Factory method
 *
 * @param key
 * @param factory
 * @return facade
 */
Facade *getFacadeInstance(const char *key, Facade *(*factory)(const char *)) {
    pthread_rwlock_wrlock(&facade_mutex);
    Facade *instance = GetFacadeMap(key);
    if (instance == NULL) {
        instance = factory(key);
        AddFacadeMap(key, instance);
        instance->initializeFacade(instance);
    }
    pthread_rwlock_unlock(&facade_mutex);
    return instance;
}
