#include "interfaces/Notifier.h"
#include "interfaces/View.h"
#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct ViewNode ViewNode;

// ViewNode LinkedList
struct ViewNode {
    const char *name;
    View *view;
    ViewNode *next;
};

// The Multiton View instanceMap.
static ViewNode *instanceMap;

// mutex for instanceMap
static pthread_rwlock_t view_mutex;

// Construct a ViewNode
static ViewNode *NewViewNode(const char *key, View *view) {
    ViewNode *node = malloc(sizeof(ViewNode));
    if (node == NULL) goto exception;
    node->name = key;
    node->view = view;
    node->next = NULL;
    return node;

    exception:
        fprintf(stderr, "ViewNode allocation failed.\n");
        return NULL;
}

// Retrieve a Node from instanceMap LinkedList
static View *GetViewNode(const char *key) {
    ViewNode *cursor = instanceMap;
    while (cursor && strcmp(cursor->name, key) != 0)
        cursor = cursor->next;
    return cursor != NULL ? cursor->view : NULL;
}

// Add ViewNode
static void AddViewMap(const char *key, View *view) {
    ViewNode **cursor = &instanceMap;
    while (*cursor)
        cursor = &(*cursor)->next;
    *cursor = NewViewNode(key, view);
}

// Remove a node from instanceMap LinkedList
static void RemoveViewMap(const char *key) {
    ViewNode **cursor = &instanceMap;
    while (*cursor) {
        if (strcmp((*cursor)->name, key) == 0) {
            ViewNode *node = *cursor;
            *cursor = (*cursor)->next;
            free(node->view);
            free(node);
            node = NULL;
            break;
        }
        cursor = &(*cursor)->next;
    }
}

struct MediatorNode {
    const char *name;
    Mediator *mediator;
    MediatorNode *next;
};

// mutex for mediatorMap
static pthread_rwlock_t mediatorMap_mutex;

// Construct a new mediatorMap node
static MediatorNode *NewMediatorNode(Mediator *mediator) {
    MediatorNode *mediatorMap = malloc(sizeof(MediatorNode));
    mediatorMap->name = mediator->getMediatorName(mediator);
    mediatorMap->mediator = mediator;
    mediatorMap->next = NULL;
    return mediatorMap;
}

// Remove a node from node LinkedList
static void DeleteMediatorNode(MediatorNode *node) {
    free(node);
    node = NULL;
}

struct ObserverNode {
    Observer *observer;
    ObserverNode *next;
};

// Construct a new ObserverNode
static ObserverNode *NewObserverNode(Observer *observer) {
    ObserverNode *self = malloc(sizeof(ObserverNode));
    self->observer = observer;
    self->next = NULL;
    return self;
}

// Delete ObserverNode
static void DeleteObserverNode(ObserverNode *observerNode) {
    DeleteObserver(observerNode->observer);
    free(observerNode);
    observerNode = NULL;
}

// Count the observers in an ObserverNode
static unsigned int CountObservers(ObserverNode *observerNode) {
    unsigned int count = 0;
    while (observerNode) {
        count++;
        observerNode = observerNode->next;
    }
    return count;
}

struct ObserverMap {
    const char *name;
    ObserverNode *observers;
    ObserverMap *next;
};

// mutex for observerMap
static pthread_rwlock_t observerMap_mutex;

// Construct a new ObserverMap
static ObserverMap *ObserverMapNew(const char *notificationName, Observer *observer) {
    ObserverMap *self = malloc(sizeof(ObserverMap));
    self->name = notificationName;
    self->observers = NewObserverNode(observer);
    self->next = NULL;
    return self;
}

// Delete ObserverMap
static void DeleteObserverMap(ObserverMap *observerMap) {
    free(observerMap);
    observerMap = NULL;
}

static void initializeView(View *self) {

}

static void registerObserver(View *self, const char *notificationName, Observer *observer) {
    pthread_rwlock_wrlock(&observerMap_mutex);
    ObserverMap **cursor = &self->observerMap;

    while (*cursor) {
        if (strcmp((*cursor)->name, notificationName) == 0) {
            ObserverNode **observers = &(*cursor)->observers;
            while (*observers)
                observers = &(*observers)->next;

            *observers = NewObserverNode(observer);
            return;
        }
        cursor = &(*cursor)->next;
    }

    *cursor = ObserverMapNew(notificationName, observer);

    pthread_rwlock_unlock(&observerMap_mutex);
}

static void notifyObservers(View *self, Notification *notification) {
    pthread_rwlock_rdlock(&observerMap_mutex);
    ObserverMap *map = self->observerMap;

    while (map) {
        if (strcmp(map->name, notification->name) == 0) {
            // Get a reference to the clone list for this notification name
            ObserverNode *cursor = map->observers;

            // Copy from reference array to working array,
            // since the reference array may change during the notification loop
            ObserverNode *clone = NULL;
            ObserverNode **iterator = &clone;
            while (cursor) {
                *iterator = NewObserverNode(NewObserver(cursor->observer->notify, cursor->observer->context));
                iterator = &(*iterator)->next;
                cursor = cursor->next;
            }

            // Notify Observers from the working array
            while (clone) {
                Observer *observer = clone->observer;
                observer->notifyObserver(observer, notification);
                ObserverNode *next = clone->next;
                DeleteObserverNode(clone);
                clone = next;
            }
            break;
        }
        map = map->next;
    }
    pthread_rwlock_unlock(&observerMap_mutex);
}

static void removeObserver(View *self, const char *notificationName, void *notifyContext) {
    pthread_rwlock_wrlock(&observerMap_mutex);
    ObserverMap **cursor = &self->observerMap;

    while (*cursor) {
        if (strcmp((*cursor)->name, notificationName) == 0) {
            // the observer list for the notification under inspection
            ObserverNode **observers = &(*cursor)->observers;

            // find the observer for the notifyContext
            while (*observers) {
                Observer *observer = (*observers)->observer;
                if (observer->compareNotifyContext(observer, notifyContext)) {
                    // there can only be one Observer for a given notifyContext
                    // in any given Observer list, so remove it and break
                    ObserverNode *node = *observers;
                    *observers = (*observers)->next; // delete observer
                    DeleteObserverNode(node);

                    // Also, when a Notification's Observer list length falls to
                    // zero, delete the notification key from the observer map
                    if (CountObservers(*observers) == 0) {
                        ObserverMap *map = *cursor;
                        *cursor = (*cursor)->next;
                        DeleteObserverMap(map);
                    }
                    return;
                }
                observers = &(*observers)->next;
            }
        }
        cursor = &(*cursor)->next;
    }
    pthread_rwlock_unlock(&observerMap_mutex);
}

static void registerMediator(View *self, Mediator *mediator) {
    pthread_rwlock_wrlock(&mediatorMap_mutex);
    MediatorNode **cursor = &self->mediatorMap;

    // do not allow re-registration (you must to removeMediator first)
    while (*cursor) {
        if (strcmp((*cursor)->name, mediator->getMediatorName(mediator)) == 0)
            return;
        cursor = &(*cursor)->next;
    }

    mediator->notifier->initializeNotifier(mediator->notifier, self->multitonKey);

    // Register the Mediator for retrieval by name
    *cursor = NewMediatorNode(mediator);

    // Get Notification interests, if any.
    const char * const *interests = mediator->listNotificationInterests(mediator);

    // Register Mediator as an observer for each notification of interests
    while(*interests) {
        // Create Observer referencing this mediator's handleNotification method
        Observer *observer = NewObserver((void (*)(void *, Notification *)) mediator->handleNotification, mediator);

        // Register Mediator as Observer for its list of Notification interests
        self->registerObserver(self, *interests, observer);
        interests++;
    }

    // alert the mediator that it has been registered
    mediator->onRegister(mediator);
    pthread_rwlock_unlock(&mediatorMap_mutex);
}

static Mediator *retrieveMediator(View *self, const char *mediatorName) {
    pthread_rwlock_rdlock(&mediatorMap_mutex);
    MediatorNode *cursor = self->mediatorMap;
    while(cursor && strcmp(cursor->name, mediatorName) != 0)
        cursor = cursor->next;
    pthread_rwlock_unlock(&mediatorMap_mutex);
    return cursor == NULL ? NULL : cursor->mediator;
}

static bool hasMediator(View *self, const char *mediatorName) {
    pthread_rwlock_rdlock(&mediatorMap_mutex);
    MediatorNode *cursor = self->mediatorMap;
    while(cursor && strcmp(cursor->name, mediatorName) != 0)
        cursor = cursor->next;
    pthread_rwlock_unlock(&mediatorMap_mutex);
    return cursor != NULL;
}

static Mediator *removeMediator(View *self, const char *mediatorName) {
    pthread_rwlock_wrlock(&mediatorMap_mutex);
    MediatorNode **cursor = &self->mediatorMap;

    while (*cursor) {
        // Retrieve the named mediator
        if (strcmp((*cursor)->name, mediatorName) == 0) {
            Mediator *mediator = (*cursor)->mediator;

            // for every notification this mediator is interested in...
            const char * const *interests = mediator->listNotificationInterests(mediator);
            while(*interests) {
                // remove the observer linking the mediator
                // to the notification interest
                self->removeObserver(self, *interests, mediator);
                interests++;
            }

            MediatorNode *node = *cursor;

            // remove the mediator from the map
            *cursor = (*cursor)->next;
            DeleteMediatorNode(node);

            // alert the mediator that it has been removed
            mediator->onRemove(mediator);
            return mediator;
        }
        cursor = &(*cursor)->next;
    }
    pthread_rwlock_unlock(&mediatorMap_mutex);
    return NULL;
}

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

View *NewView(const char *key) {
    assert(GetViewNode(key) == NULL);

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

void RemoveView(const char *key) {
    pthread_rwlock_wrlock(&view_mutex);
    RemoveViewMap(key);
    pthread_rwlock_unlock(&view_mutex);
}

View *getViewInstance(const char *key, View *(*factory)(const char *)) {
    pthread_rwlock_wrlock(&view_mutex);
    View *instance = GetViewNode(key);
    if (instance == NULL) {
        instance = factory(key);
        instance->initializeView(instance);
    }
    pthread_rwlock_unlock(&view_mutex);
    return instance;
}