#include "interfaces/Notifier.h"
#include "interfaces/View.h"
#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ViewMap LinkedList
typedef struct ViewMap ViewMap;

struct ViewMap {
    const char *name;
    View *view;
    ViewMap *next;
};

// The Multiton View instanceMap.
static ViewMap *instanceMap;

// mutex for instanceMap
static pthread_rwlock_t view_mutex;

/**
 * Constructor
 *
 * @param key
 * @param view
 * @return ViewMap*
 */
static ViewMap *NewViewMap(const char *key, View *view) {
    ViewMap *viewMap = malloc(sizeof(ViewMap));
    if (viewMap == NULL) goto exception;
    viewMap->name = key;
    viewMap->view = view;
    viewMap->next = NULL;
    return viewMap;

    exception:
        fprintf(stderr, "ViewMap allocation failed.\n");
        return NULL;
}

/**
 * Retrieve a Node from instanceMap LinkedList
 *
 * @param key
 * @return View*
 */
static View *GetViewMap(const char *key) {
    ViewMap *viewMap = instanceMap;
    while (viewMap && strcmp(viewMap->name, key) != 0)
        viewMap = viewMap->next;
    return viewMap == NULL ? NULL : viewMap->view;
}

/**
 * Add ViewMap
 *
 * @param key
 * @param view
 */
static void AddViewMap(const char *key, View *view) {
    ViewMap **viewMap = &instanceMap;
    while (*viewMap)
        viewMap = &(*viewMap)->next;
    *viewMap = NewViewMap(key, view);
}

/**
 * Remove a node from instanceMap LinkedList
 *
 * @param key
 */
static void RemoveViewMap(const char *key) {
    ViewMap **viewMap = &instanceMap;
    while (*viewMap) {
        if (strcmp((*viewMap)->name, key) == 0) {
            ViewMap *node = *viewMap;
            *viewMap = (*viewMap)->next;
            free(node->view);
            free(node);
            node = NULL;
            break;
        }
        viewMap = &(*viewMap)->next;
    }
}

// mutex for mediatorMap
static pthread_rwlock_t mediatorMap_mutex;

/**
 * Construct a new mediatorMap node
 *
 * @param mediator
 * @return
 */
static MediatorMap *NewMediatorMap(Mediator *mediator) {
    MediatorMap *mediatorMap = malloc(sizeof(MediatorMap));
    mediatorMap->name = mediator->getMediatorName(mediator);
    mediatorMap->mediator = mediator;
    mediatorMap->next = NULL;
    return mediatorMap;
}

/**
 * Remove a node from mediatorMap LinkedList
 *
 * @param mediatorMap
 */
static void DeleteMediatorMap(MediatorMap *mediatorMap) {
    free(mediatorMap);
    mediatorMap = NULL;
}

/**
 * Construct a new ObserverNode
 *
 * @param observer
 * @return
 */
static ObserverNode *NewObserverNode(Observer *observer) {
    ObserverNode *self = malloc(sizeof(ObserverNode));
    self->observer = observer;
    self->next = NULL;
    return self;
}

/**
 * Delete ObserverNode
 *
 * @param observerNode
 */
static void DeleteObserverNode(ObserverNode *observerNode) {
    DeleteObserver(observerNode->observer);
    free(observerNode);
    observerNode = NULL;
}

/**
 * Count the observers in an ObserverNode
 *
 * @param observerNode
 * @return
 */
static int CountObservers(ObserverNode *observerNode) {
    int count = 0;
    while (observerNode) {
        count++;
        observerNode = observerNode->next;
    }
    return count;
}

// mutex for observerMap
static pthread_rwlock_t observerMap_mutex;

/**
 * Construct a new ObserverMap
 *
 * @param notificationName
 * @param observer
 * @return
 */
static ObserverMap *ObserverMapNew(const char *notificationName, Observer *observer) {
    ObserverMap *self = malloc(sizeof(ObserverMap));
    self->name = notificationName;
    self->observers = NewObserverNode(observer);
    self->next = NULL;
    return self;
}

/**
 *
 * @param observerMap
 */
static void DeleteObserverMap(ObserverMap *observerMap) {
    free(observerMap);
    observerMap = NULL;
}

/**
 * <P>Initialize the Singleton View instance.</P>
 *
 * <P>Called automatically by the constructor, this
 * is your opportunity to initialize the Singleton
 * instance in your subclass without overriding the
 * constructor.</P>
 */
static void initializeView(View *self) {

}

/**
 * <P>Register an <code>Observer</code> to be notified
 * of <code>Notifications</code> with a given name.</P>
 *
 * @param View*
 * @param notificationName the name of the <code>Notifications</code> to notify this <code>Observer</code> of
 * @param observer the <code>Observer</code> to register
 */
static void registerObserver(View *self, const char *notificationName, Observer *observer) {
    pthread_rwlock_wrlock(&observerMap_mutex);
    ObserverMap **observerMap = &self->observerMap;

    while (*observerMap) {
        if (strcmp((*observerMap)->name, notificationName) == 0) {
            ObserverNode **observers = &(*observerMap)->observers;
            while (*observers)
                observers = &(*observers)->next;

            *observers = NewObserverNode(observer);
            return;
        }
        observerMap = &(*observerMap)->next;
    }

    *observerMap = ObserverMapNew(notificationName, observer);

    pthread_rwlock_unlock(&observerMap_mutex);
}

/**
 * <P>Notify the <code>Observers</code> for a particular <code>Notification</code>.</P>
 *
 * <P>All previously attached <code>Observers</code> for this <code>Notification</code>'s
 * list are notified and are passed a reference to the <code>Notification</code> in
 * the order in which they were registered.</P>
 *
 * @param self
 * @param notification the <code>Notification</code> to notify <code>Observers</code> of.
 */
static void notifyObservers(View *self, Notification *notification) {
    pthread_rwlock_rdlock(&observerMap_mutex);
    ObserverMap *pointer = self->observerMap;

    while (pointer) {
        if (strcmp(pointer->name, notification->name) == 0) {
            // Get a reference to the observers list for this notification name
            ObserverNode *cursor = pointer->observers;

            // Copy observers from reference array to working array,
            // since the reference array may change during the notification loop
            ObserverNode *observers = NULL;
            ObserverNode **previous = &observers;
            while (cursor) {
                Observer *observer = NewObserver(cursor->observer->notify, cursor->observer->context);
                *previous = NewObserverNode(observer);
                previous = &(*previous)->next;
                cursor = cursor->next;
            }

            // Notify Observers from the working array
            while (observers) {
                Observer *observer = observers->observer;
                observer->notifyObserver(observer, notification);
                ObserverNode *next = observers->next;
                DeleteObserverNode(observers);
                observers = next;
            }
            break;
        }
        pointer = pointer->next;
    }
    pthread_rwlock_unlock(&observerMap_mutex);
}

/**
 * <P>Remove the observer for a given notifyContext from an observer list for a given Notification name.</P>
 *
 * @param self
 * @param notificationName which observer list to remove from
 * @param notifyContext remove the observer with this object as its notifyContext
 */
static void removeObserver(View *self, const char *notificationName, void *notifyContext) {
    pthread_rwlock_wrlock(&observerMap_mutex);
    ObserverMap **observerMap = &self->observerMap;

    while (*observerMap) {
        if (strcmp((*observerMap)->name, notificationName) == 0) {
            ObserverNode **observers = &(*observerMap)->observers;
            while (*observers) {
                Observer *observer = (*observers)->observer;
                if (observer->compareNotifyContext(observer, notifyContext)) {

                    ObserverNode *node = *observers;
                    *observers = (*observers)->next; // delete observer
                    DeleteObserverNode(node);

                    if (CountObservers(*observers) == 0) {  // if empty
                        ObserverMap *map = *observerMap;
                        *observerMap = (*observerMap)->next;
                        DeleteObserverMap(map);
                    }

                    return;
                }
                observers = &(*observers)->next;
            }
        }
        observerMap = &(*observerMap)->next;
    }
    pthread_rwlock_unlock(&observerMap_mutex);
}

/**
 * <P>Register an <code>Mediator</code> instance with the <code>View</code>.</P>
 *
 * <P>Registers the <code>Mediator</code> so that it can be retrieved by name,
 * and further interrogates the <code>Mediator</code> for its
 * <code>Notification</code> interests.</P>
 *
 * <P>If the <code>Mediator</code> returns any <code>Notification</code>
 * names to be notified about, an <code>Observer</code> is created encapsulating
 * the <code>Mediator</code> instance's <code>handleNotification</code> method
 * and registering it as an <code>Observer</code> for all <code>Notifications</code> the
 * <code>Mediator</code> is interested in.</P>
 *
 * @param self
 * @param mediator a reference to the <code>Mediator</code> instance
 */
static void registerMediator(View *self, Mediator *mediator) {
    pthread_rwlock_wrlock(&mediatorMap_mutex);
    MediatorMap **mediatorMap = &self->mediatorMap;

    while (*mediatorMap) {
        if (strcmp((*mediatorMap)->name, mediator->getMediatorName(mediator)) == 0)
            return; // do not allow re-registration (you must to removeMediator first)
        mediatorMap = &(*mediatorMap)->next;
    }

    mediator->notifier->initializeNotifier(mediator->notifier, self->multitonKey);
    *mediatorMap = NewMediatorMap(mediator);

    const char * const *interests = mediator->listNotificationInterests(mediator);
    while(*interests) {
        Observer *observer = NewObserver((void (*)(void *, Notification *)) mediator->handleNotification, mediator);
        self->registerObserver(self, *interests, observer);
        interests++;
    }

    mediator->onRegister(mediator);
    pthread_rwlock_unlock(&mediatorMap_mutex);
}

/**
 * <P>Retrieve an <code>Mediator</code> from the <code>View</code>.</P>
 *
 * @param self
 * @param mediatorName the name of the <code>Mediator</code> instance to retrieve.
 * @return the <code>Mediator</code> instance previously registered with the given <code>mediatorName</code>.
 */
static Mediator *retrieveMediator(View *self, const char *mediatorName) {
    pthread_rwlock_rdlock(&mediatorMap_mutex);
    MediatorMap *mediatorMap = self->mediatorMap;
    while(mediatorMap && strcmp(mediatorMap->name, mediatorName) != 0)
        mediatorMap = mediatorMap->next;
    pthread_rwlock_unlock(&mediatorMap_mutex);
    return mediatorMap == NULL ? NULL : mediatorMap->mediator;
}

/**
 * <P>Check if a Mediator is registered or not</P>
 *
 * @param self
 * @param mediatorName mediator name
 * @return whether a Mediator is registered with the given <code>mediatorName</code>.
 */
static bool hasMediator(View *self, const char *mediatorName) {
    pthread_rwlock_rdlock(&mediatorMap_mutex);
    MediatorMap *mediatorMap = self->mediatorMap;
    while(mediatorMap && strcmp(mediatorMap->name, mediatorName) != 0)
        mediatorMap = mediatorMap->next;
    pthread_rwlock_unlock(&mediatorMap_mutex);
    return mediatorMap != NULL;
}

/**
 * <P>Remove an <code>Mediator</code> from the <code>View</code>.</P>
 *
 * @param self
 * @param mediatorName name of the <code>Mediator</code> instance to be removed.
 * @return the <code>Mediator</code> that was removed from the <code>View</code>
 */
static Mediator *removeMediator(View *self, const char *mediatorName) {
    pthread_rwlock_wrlock(&mediatorMap_mutex);
    MediatorMap **mediatorMap = &self->mediatorMap;

    while (*mediatorMap) {
        if (strcmp((*mediatorMap)->name, mediatorName) == 0) {
            Mediator *mediator = (*mediatorMap)->mediator;

            const char * const *interests = mediator->listNotificationInterests(mediator);
            while(*interests) {
                self->removeObserver(self, *interests, mediator);
                interests++;
            }

            MediatorMap *node = *mediatorMap;
            *mediatorMap = (*mediatorMap)->next; // remove the mediator from the map
            DeleteMediatorMap(node);

            // alert the mediator that it has been removed
            mediator->onRemove(mediator);
            return mediator;
        }
        mediatorMap = &(*mediatorMap)->next;
    }
    pthread_rwlock_unlock(&mediatorMap_mutex);
    return NULL;
}

/**
 * Initializer
 *
 * @param view
 */
void InitView(View *view) {
    view->mediatorMap = NULL;
    view->observerMap = NULL;
    view->initializeView = initializeView;
    view->registerObserver = registerObserver;
    view->notifyObservers = notifyObservers;
    view->removeObserver = removeObserver;
    view->registerMediator = registerMediator;
    view->retrieveMediator = retrieveMediator;
    view->hasMediator = hasMediator;
    view->removeMediator = removeMediator;
}

/**
 * <P>Constructor.</P>
 *
 * <P>This <code>View</code> implementation is a Multiton,
 * so you should not call the constructor
 * directly, but instead call the static Multiton
 * Factory method <code>View.getInstance( multitonKey )</code></P>
 *
 * @param key multitonKey
 * @throws Error if instance for this Multiton key has already been constructed
 * @returns View*
 */
View *NewView(const char *key) {
    assert(GetViewMap(key) == NULL);

    View *view = malloc(sizeof(View));
    if (view == NULL) goto exception;
    InitView(view);
    view->multitonKey = key;
    AddViewMap(key, view);
    return view;

    exception:
        fprintf(stderr, "View allocation failed.\n");
        return NULL;
}

/**
 * <P>Remove an View instance</P>
 *
 * @param key of View instance to remove
 */
void RemoveView(const char *key) {
    pthread_rwlock_wrlock(&view_mutex);
    RemoveViewMap(key);
    pthread_rwlock_unlock(&view_mutex);
}

/**
 * <P>View Singleton Factory method.</P>
 *
 * @param key multitonKey
 * @param factory factory that returns <code>View*</code>
 * @return the Multiton instance of <code>View*</code>
 */
View *getViewInstance(const char *key, View *(*factory)(const char *)) {
    pthread_rwlock_wrlock(&view_mutex);
    View *instance = GetViewMap(key);
    if (instance == NULL) {
        instance = factory(key);
        instance->initializeView(instance);
    }
    pthread_rwlock_unlock(&view_mutex);
    return instance;
}