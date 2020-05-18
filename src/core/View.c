#include "interfaces/Notifier.h"
#include "interfaces/View.h"
#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct ViewMap ViewMap;

// ViewMap LinkedList
struct ViewMap {
    const char *name;
    View *view;
    ViewMap *next;
};

// The Multiton View instanceMap.
static ViewMap *instanceMap;

// mutex for instanceMap
static pthread_rwlock_t view_mutex;

// Construct a ViewMap
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

// Retrieve a Node from instanceMap LinkedList
static View *GetViewMap(const char *key) {
    ViewMap *viewMap = instanceMap;
    while (viewMap && strcmp(viewMap->name, key) != 0)
        viewMap = viewMap->next;
    return viewMap == NULL ? NULL : viewMap->view;
}

// Add ViewMap
static void AddViewMap(const char *key, View *view) {
    ViewMap **viewMap = &instanceMap;
    while (*viewMap)
        viewMap = &(*viewMap)->next;
    *viewMap = NewViewMap(key, view);
}

// Remove a node from instanceMap LinkedList
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

struct MediatorMap {
    const char *name;
    Mediator *mediator;
    MediatorMap *next;
};

// mutex for mediatorMap
static pthread_rwlock_t mediatorMap_mutex;

// Construct a new mediatorMap node
static MediatorMap *NewMediatorMap(Mediator *mediator) {
    MediatorMap *mediatorMap = malloc(sizeof(MediatorMap));
    mediatorMap->name = mediator->getMediatorName(mediator);
    mediatorMap->mediator = mediator;
    mediatorMap->next = NULL;
    return mediatorMap;
}

// Remove a node from mediatorMap LinkedList
static void DeleteMediatorMap(MediatorMap *mediatorMap) {
    free(mediatorMap);
    mediatorMap = NULL;
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

static void removeObserver(View *self, const char *notificationName, void *notifyContext) {
    pthread_rwlock_wrlock(&observerMap_mutex);
    ObserverMap **observerMap = &self->observerMap;

    while (*observerMap) {
        if (strcmp((*observerMap)->name, notificationName) == 0) {
            // the observer list for the notification under inspection
            ObserverNode **observers = &(*observerMap)->observers;

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

static void registerMediator(View *self, Mediator *mediator) {
    pthread_rwlock_wrlock(&mediatorMap_mutex);
    MediatorMap **mediatorMap = &self->mediatorMap;

    // do not allow re-registration (you must to removeMediator first)
    while (*mediatorMap) {
        if (strcmp((*mediatorMap)->name, mediator->getMediatorName(mediator)) == 0)
            return;
        mediatorMap = &(*mediatorMap)->next;
    }

    mediator->notifier->initializeNotifier(mediator->notifier, self->multitonKey);

    // Register the Mediator for retrieval by name
    *mediatorMap = NewMediatorMap(mediator);

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
    MediatorMap *mediatorMap = self->mediatorMap;
    while(mediatorMap && strcmp(mediatorMap->name, mediatorName) != 0)
        mediatorMap = mediatorMap->next;
    pthread_rwlock_unlock(&mediatorMap_mutex);
    return mediatorMap == NULL ? NULL : mediatorMap->mediator;
}

static bool hasMediator(View *self, const char *mediatorName) {
    pthread_rwlock_rdlock(&mediatorMap_mutex);
    MediatorMap *mediatorMap = self->mediatorMap;
    while(mediatorMap && strcmp(mediatorMap->name, mediatorName) != 0)
        mediatorMap = mediatorMap->next;
    pthread_rwlock_unlock(&mediatorMap_mutex);
    return mediatorMap != NULL;
}

static Mediator *removeMediator(View *self, const char *mediatorName) {
    pthread_rwlock_wrlock(&mediatorMap_mutex);
    MediatorMap **mediatorMap = &self->mediatorMap;

    while (*mediatorMap) {
        // Retrieve the named mediator
        if (strcmp((*mediatorMap)->name, mediatorName) == 0) {
            Mediator *mediator = (*mediatorMap)->mediator;

            // for every notification this mediator is interested in...
            const char * const *interests = mediator->listNotificationInterests(mediator);
            while(*interests) {
                // remove the observer linking the mediator
                // to the notification interest
                self->removeObserver(self, *interests, mediator);
                interests++;
            }

            MediatorMap *node = *mediatorMap;

            // remove the mediator from the map
            *mediatorMap = (*mediatorMap)->next;
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

void RemoveView(const char *key) {
    pthread_rwlock_wrlock(&view_mutex);
    RemoveViewMap(key);
    pthread_rwlock_unlock(&view_mutex);
}

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