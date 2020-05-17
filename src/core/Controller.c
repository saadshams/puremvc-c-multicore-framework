#include "interfaces/Controller.h"
#include "interfaces/Notifier.h"
#include "interfaces/View.h"
#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ControllerMap LinkedList
typedef struct ControllerMap ControllerMap;

struct ControllerMap {
    const char *name;
    Controller *controller;
    ControllerMap *next;
};

// The Multiton Controller instanceMap.
static ControllerMap *instanceMap;

/**
 * Construct a new instanceMap node
 *
 * @param key
 * @param controller
 * @return
 */
static ControllerMap *NewControllerMap(const char *key, Controller *controller) {
    ControllerMap *controllerMap = malloc(sizeof(ControllerMap));
    if (controllerMap == NULL) goto exception;
    controllerMap->name = key;
    controllerMap->controller = controller;
    controllerMap->next = NULL;
    return controllerMap;

    exception:
        fprintf(stderr, "ControllerMap allocation failed.\n");
        return NULL;
}

/**
 * Retrieve a Node from instanceMap LinkedList
 *
 * @param key
 * @return controller
 */
static Controller *GetControllerMap(const char *key) {
    ControllerMap *controllerMap = instanceMap;
    while (controllerMap && strcmp(controllerMap->name, key) != 0)
        controllerMap = controllerMap->next;
    return controllerMap == NULL ? NULL : controllerMap->controller;
}

/**
 * Add a Node to the instanceMap LinkedList
 *
 * @param key
 * @param controller
 */
static void AddControllerMap(const char *key, Controller *controller) {
    ControllerMap **controllerMap = &instanceMap;
    while (*controllerMap)
        controllerMap = &(*controllerMap)->next;
    *controllerMap = NewControllerMap(key, controller);
}

/**
 * Remove a node from instanceMap LinkedList
 *
 * @param key
 */
static void RemoveControllerMap(const char *key) {
    ControllerMap **controllerMap = &instanceMap;
    while (*controllerMap) {
        if (strcmp((*controllerMap)->name, key) == 0) {
            ControllerMap *node = *controllerMap;
            *controllerMap = (*controllerMap)->next;
            free(node->controller);
            free(node);
            node = NULL;
            break;
        }
        controllerMap = &(*controllerMap)->next;
    }
}

struct CommandMap {
    const char *name;
    SimpleCommand *(*factory)();
    CommandMap *next;
};


// mutex for commandMap
static pthread_rwlock_t commandMap_mutex = PTHREAD_MUTEX_INITIALIZER;

/**
 * Construct a new commandMap node
 *
 * @param notificationName
 * @param factory
 * @return CommandMap*
 */
static CommandMap *NewCommandMap(const char *notificationName, SimpleCommand *(*factory)()) {
    CommandMap *self = malloc(sizeof(CommandMap));
    self->name = notificationName;
    self->factory = factory;
    self->next = NULL;
    return self;
}

/**
 * Release the memory for a commandMap node
 *
 * @param CommandMap*
 */
static void DeleteCommandMap(CommandMap *self) {
    free(self);
    self = NULL;
}

/**
 * <P>Initialize the Multiton <code>Controller</code> instance.</P>
 *
 * <P>Called automatically by the constructor.</P>
 *
 * <P>Note that if you are using a subclass of <code>View</code>
 * in your application, you should <i>also</i> subclass <code>Controller</code>
 * and override the <code>initializeController</code> method in the
 * following way:</P>
 *
 * <code>// ensure that the Controller is talking to my View implementation
 * static void initializeController()
 * {
 *     view = getViewInstance(self->multitonKey, NewMyView);
 * }
 * </code>
 */
static void initializeController(Controller *self) {
    self->view = getViewInstance(self->multitonKey, NewView);
}

/**
 * <P>If an <code>Command</code> has previously been registered
 * to handle a the given <code>Notification</code>, then it is executed.</P>
 *
 * @param self
 * @param notification an <code>Notification</code>
 */
static void executeCommand(Controller *self, Notification *notification) {
    pthread_rwlock_rdlock(&commandMap_mutex);
    CommandMap *cursor = self->commandMap;
    while(cursor && strcmp(cursor->name, notification->getName(notification)) != 0)
        cursor = cursor->next;

    if (cursor) {
        SimpleCommand *commandInstance = cursor->factory();
        commandInstance->notifier->initializeNotifier(commandInstance->notifier, self->multitonKey);
        commandInstance->execute(commandInstance, notification);
        DeleteSimpleCommand(commandInstance);
    }
    pthread_rwlock_unlock(&commandMap_mutex);
}

/**
 * <P>Register a particular <code>Command</code> class as the handler
 * for a particular <code>Notification</code>.</P>
 *
 * <P>If an <code>Command</code> has already been registered to
 * handle <code>Notification</code>s with this name, it is no longer
 * used, the new <code>Command</code> is used instead.</P>
 *
 * <P>The Observer for the new Command is only created if this the
 * first time an Command has been registered for this Notification name.</P>
 *
 * @param self
 * @param notificationName the name of the <code>Notification</code>
 * @param factory a reference to <code>Command</code> factory
 */
static void registerCommand(Controller *self, const char *notificationName, SimpleCommand *(factory)()) {
    pthread_rwlock_wrlock(&commandMap_mutex);
    CommandMap **cursor = &self->commandMap;

    while (*cursor) {
        if (strcmp((*cursor)->name, notificationName) == 0) {
            CommandMap *node = NewCommandMap(notificationName, factory);
            node->next = (*cursor)->next;
            DeleteCommandMap(*cursor);
            *cursor = node;
            return;
        }
        cursor = &(*cursor)->next;
    }

    *cursor = NewCommandMap(notificationName, factory);
    Observer *observer = NewObserver((void (*)(void *, Notification *)) self->executeCommand, self);
    self->view->registerObserver(self->view, notificationName, observer);
    pthread_rwlock_unlock(&commandMap_mutex);
}

/**
 * <P>Check if a Command is registered for a given Notification</P>
 *
 * @param self
 * @param notificationName notification name
 * @return whether a Command is currently registered for the given <code>notificationName</code>.
 */
static bool hasCommand(Controller *self, const char *notificationName) {
    pthread_rwlock_rdlock(&commandMap_mutex);
    CommandMap *cursor = self->commandMap;
    while(cursor != NULL && strcmp(cursor->name, notificationName) != 0)
        cursor = cursor->next;
    pthread_rwlock_unlock(&commandMap_mutex);
    return cursor != NULL;
}

/**
 * <P>Remove a previously registered <code>Command</code> to <code>Notification</code> mapping.</P>
 *
 * @param self
 * @param notificationName the name of the <code>Notification</code> to remove the <code>Command</code> mapping for
 */
static void removeCommand(Controller *self, const char *notificationName) {
    pthread_rwlock_wrlock(&commandMap_mutex);
    CommandMap **cursor = &self->commandMap;

    while (*cursor) {
        if (strcmp((*cursor)->name, notificationName) == 0) {
            CommandMap *commandMap = (*cursor);
            *cursor = (*cursor)->next;
            self->view->removeObserver(self->view, notificationName, self);
            DeleteCommandMap(commandMap);
            break;
        }
        cursor = &(*cursor)->next;
    }
    pthread_rwlock_unlock(&commandMap_mutex);
}

/**
 * Initializer
 *
 * @param controller
 */
void InitController(Controller *controller) {
    controller->commandMap = NULL;
    controller->initializeController = initializeController;
    controller->executeCommand = executeCommand;
    controller->registerCommand = registerCommand;
    controller->hasCommand = hasCommand;
    controller->removeCommand = removeCommand;
}

// mutex for controller instanceMap
static pthread_rwlock_t controller_mutex;

/**
 * Constructor
 *
 * <P>This <code>Controller</code> implementation is a Multiton,
 * so you should not call the constructor
 * directly, but instead call the static Factory method,
 * passing the unique key and a supplier for this instance
 * <code>getControllerInstance(multitonKey, NewController)</code></P>
 *
 * @param key
 * @return instance of <code>Controller</code>
 * @throws Error if instance for this Multiton key has already been constructed
 */
Controller *NewController(const char *key) {
    assert(GetControllerMap(key) == NULL);

    Controller *controller = malloc(sizeof(Controller));
    if (controller == NULL) goto exception;
    InitController(controller);
    controller->multitonKey = key;
    AddControllerMap(key, controller);
    return controller;

    exception:
        fprintf(stderr, "Controller allocation failed.\n");
        return NULL;
}

/**
 * Destructor
 *
 * @param key
 */
void RemoveController(const char *key) {
    pthread_rwlock_wrlock(&controller_mutex);
    RemoveControllerMap(key);
    pthread_rwlock_unlock(&controller_mutex);
}

/**
 * <P><code>Controller</code> Multiton Factory method.</P>
 *
 * @param key multitonKey
 * @param factory factory that returns <code>Controller</code>
 * @return the Multiton instance of <code>Controller</code>
 */
Controller *getControllerInstance(const char *key, Controller *(*factory)(const char *)) {
    pthread_rwlock_wrlock(&controller_mutex);
    Controller *instance = GetControllerMap(key);
    if (instance == NULL) {
        instance = factory(key);
        instance->initializeController(instance);
    }
    pthread_rwlock_unlock(&controller_mutex);
    return instance;
}
