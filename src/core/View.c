#include "interfaces/Notifier.h"
#include "interfaces/View.h"
#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static pthread_mutex_t view_mutex;

static pthread_rwlock_t mediatorMap_mutex;

static pthread_rwlock_t observerMap_mutex;

// ViewMap

static ViewMap *instanceMap;

static ViewMap *NewViewMap(const char *key, View *view) {
    ViewMap *self = malloc(sizeof(ViewMap));
    if (self == NULL) goto exception;
    self->name = key;
    self->view = view;
    self->next = NULL;
    return self;

    exception:
        fprintf(stderr, "ViewMap allocation failed.\n");
        return NULL;
}

static void AddViewMap(const char *key, View *view) {
    ViewMap **viewMap = &instanceMap;
    while (*viewMap)
        viewMap = &(*viewMap)->next;
    *viewMap = NewViewMap(key, view);
}

static View *GetViewMap(const char *key) {
    ViewMap *viewMap = instanceMap;
    while (viewMap && strcmp(viewMap->name, key) != 0)
        viewMap = viewMap->next;
    return viewMap == NULL ? NULL : viewMap->view;
}

static void DeleteViewMap(ViewMap *self) {
    free(self->view->multitonKey);
    free(self->view);
    free(self);
    self = NULL;
}

static void RemoveViewMap(const char *key) {
    ViewMap **viewMap = &instanceMap;
    while (*viewMap) {
        if (strcmp((*viewMap)->name, key) == 0) {
            ViewMap *node = *viewMap;
            *viewMap = (*viewMap)->next;
            DeleteViewMap(node);
            break;
        }
        viewMap = &(*viewMap)->next;
    }
}

// MediatorMap

static MediatorMap *NewMediatorMap(Mediator *mediator) {
    MediatorMap *self = malloc(sizeof(MediatorMap));
    self->name = strdup(mediator->getMediatorName(mediator));
    self->mediator = mediator;
    self->next = NULL;
    return self;
}

static void DeleteMediatorMap(MediatorMap *self) {
    free(self->name);
    self->mediator = NULL;
    self->next = NULL;
    free(self);
    self = NULL;
}

// ObserverNode

static ObserverNode *NewObserverNode(Observer *observer) {
    ObserverNode *self = malloc(sizeof(ObserverNode));
    self->observer = observer;
    self->next = NULL;
    return self;
}

static void DeleteObserverNode(ObserverNode *self) {
    DeleteObserver(self->observer);
    free(self);
    self = NULL;
}

static int CountObservers(ObserverNode *self) {
    int count = 0;
    while (self) {
        count++;
        self = self->next;
    }
    return count;
}

// ObserverMap

static ObserverMap *ObserverMapNew(const char *notificationName, Observer *observer) {
    ObserverMap *self = malloc(sizeof(ObserverMap));
    self->name = strdup(notificationName);
    self->observers = NewObserverNode(observer);
    self->next = NULL;
    return self;
}

static void DeleteObserverMap(ObserverMap *self) {
    free(self->name);
    self->name = NULL;
    self->observers = NULL;
    free(self);
}

// View

static void initializeView(View *self) {

}

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

void InitView(View *self) {
    self->mediatorMap = NULL;
    self->observerMap = NULL;
    self->initializeView = initializeView;
    self->registerObserver = registerObserver;
    self->notifyObservers = notifyObservers;
    self->removeObserver = removeObserver;
    self->registerMediator = registerMediator;
    self->retrieveMediator = retrieveMediator;
    self->hasMediator = hasMediator;
    self->removeMediator = removeMediator;
}

View *NewView(const char *key) {
    assert(GetViewMap(key) == NULL);
    View *self = malloc(sizeof(View));
    if (self == NULL) goto exception;
    InitView(self);
    self->multitonKey = strdup(key);
    return self;

    exception:
        fprintf(stderr, "View allocation failed.\n");
        return NULL;
}

void DeleteView(const char *key) {
    pthread_mutex_lock(&view_mutex);
    RemoveViewMap(key);
    pthread_mutex_unlock(&view_mutex);
}

View *getViewInstance(const char *key, View *(*factory)(const char *)) {
    pthread_mutex_lock(&view_mutex);
    View *instance = GetViewMap(key);
    if (instance == NULL) {
        instance = factory(key);
        AddViewMap(key, instance);
        instance->initializeView(instance);
    }
    pthread_mutex_unlock(&view_mutex);
    return instance;
}