#include "interfaces/View.h"
#include "interfaces/Notifier.h"
#include <pthread.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

static pthread_mutex_t view_mutex = PTHREAD_MUTEX_INITIALIZER;

static pthread_rwlock_t mediatorMap_mutex = PTHREAD_MUTEX_INITIALIZER;

static pthread_rwlock_t observerMap_mutex = PTHREAD_MUTEX_INITIALIZER;

static ViewMap *instanceMap;

static ViewMap *NewViewMap(char *key, View *view) {
    ViewMap *self = malloc(sizeof(ViewMap));
    self->name = strdup(key);
    self->view = view;
    self->next = NULL;
    return self;
}

static void AddViewMap(char *key, View *view) {
    ViewMap **previous = &instanceMap;
    while (*previous)
        previous = &(*previous)->next;

    *previous = NewViewMap(key, view);
}

static View *GetViewMap(char *key) {
    ViewMap *cursor = instanceMap;
    while (cursor && strcmp(cursor->name, key) != 0)
        cursor = cursor->next;
    return cursor == NULL ? NULL : cursor->view;
}

static void RemoveViewMap(char *key) {
    ViewMap **cursor = &instanceMap;
    while (*cursor) {
        if (strcmp((*cursor)->name, key) == 0) {
            *cursor = (*cursor)->next;
            free((*cursor)->view);
            free((*cursor));
            break;
        }
        cursor = &(*cursor)->next;
    }
}

static MediatorMap *NewMediatorMap(Mediator *mediator) {
    MediatorMap *self = malloc(sizeof(MediatorMap));
    self->name = strdup(mediator->getMediatorName(mediator));
    self->mediator = mediator;
    self->next = NULL;
    return self;
}

static void DeleteMediatorMap(MediatorMap *self) {
    free(self->name);
    free(self);
}

static ObserverNode *NewObserverNode(Observer *observer) {
    ObserverNode *self = malloc(sizeof(ObserverNode));
    self->observer = observer;
    self->next = NULL;
    return self;
}

static void DeleteObserverNode(ObserverNode *self) {
    free(self->observer);
    free(self);
}

static ObserverMap *ObserverMapNew(const char *notificationName, Observer *observer) {
    ObserverMap *self = malloc(sizeof(ObserverMap));
    self->name = strdup(notificationName);
    self->observers = NewObserverNode(observer);
    self->next = NULL;
    return self;
}

static void DeleteObserverMap(ObserverMap *self) {
    free(self->name);
    free(self);
}

static void initializeView(View *self) {

}

static void registerMediator(View *self, Mediator *mediator) {
    pthread_rwlock_wrlock(&mediatorMap_mutex);
    MediatorMap **cursor = &self->mediatorMap;

    while (*cursor) {
        if (strcmp((*cursor)->name, mediator->getMediatorName(mediator)) == 0)
            return; // do not allow re-registration (you must to removeMediator first)
        cursor = &(*cursor)->next;
    }

    mediator->notifier->initializeNotifier(mediator->notifier, self->multitonKey);
    *cursor = NewMediatorMap(mediator);

    char **interests = mediator->listNotificationInterests(mediator);
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
    MediatorMap *cursor = self->mediatorMap;
    while(cursor && strcmp(cursor->name, mediatorName) != 0)
        cursor = cursor->next;
    pthread_rwlock_unlock(&mediatorMap_mutex);
    return cursor == NULL ? NULL : cursor->mediator;
}

static bool hasMediator(View *self, const char *mediatorName) {
    pthread_rwlock_rdlock(&mediatorMap_mutex);
    MediatorMap *cursor = self->mediatorMap;
    while(cursor && strcmp(cursor->name, mediatorName) != 0)
        cursor = cursor->next;
    pthread_rwlock_unlock(&mediatorMap_mutex);
    return cursor != NULL;
}

static Mediator *removeMediator(View *self, const char *mediatorName) {
    pthread_rwlock_wrlock(&mediatorMap_mutex);
    MediatorMap **cursor = &self->mediatorMap;

    while (*cursor) {
        if (strcmp((*cursor)->name, mediatorName) == 0) {
            Mediator *mediator = (*cursor)->mediator;
            char **interests = mediator->listNotificationInterests(mediator);
            while(*interests) {
                self->removeObserver(self, *interests, mediator);
                interests++;
            }
            *cursor = (*cursor)->next; // remove the mediator from the map

            // alert the mediator that it has been removed
            mediator->onRemove(mediator);
            return mediator;
        }
        cursor = &(*cursor)->next;
    }
    pthread_rwlock_unlock(&mediatorMap_mutex);
    return NULL;
}

static void registerObserver(View *self, const char *notificationName, Observer *observer) {
    pthread_rwlock_wrlock(&observerMap_mutex);
    ObserverMap **pointer = &self->observerMap;

    while (*pointer) {
        if (strcmp((*pointer)->name, notificationName) == 0) {
            ObserverNode **cursor = &(*pointer)->observers;
            while (*cursor)
                cursor = &(*cursor)->next;

            *cursor = NewObserverNode(observer);
            return;
        }
        pointer = &(*pointer)->next;
    }

    *pointer = ObserverMapNew(notificationName, observer);
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
                *previous = NewObserverNode(NewObserver(cursor->observer->notify, cursor->observer->context));
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
    ObserverMap *pointer = self->observerMap;

    while (pointer) {
        if (strcmp(pointer->name, notificationName) == 0) {
            ObserverNode **cursor = &pointer->observers;
            while (*cursor) {
                Observer *observer = (*cursor)->observer;
                if (observer->compareNotifyContext(observer, notifyContext)) {
                    DeleteObserverNode(*cursor);
                    *cursor = (*cursor)->next;
                    if (self->observerMap->observers == NULL) { // if empty
                        DeleteObserverMap(self->observerMap);
                        self->observerMap = NULL;
                    }
                    return;
                }
                cursor = &(*cursor)->next;
            }
        }
        pointer = pointer->next;
    }
    pthread_rwlock_unlock(&observerMap_mutex);
}

void InitView(View *self) {
    if (self) {
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
}

View *NewView(char *key) {
    assert(GetViewMap(key) == NULL);
    View *self = malloc(sizeof(View));
    InitView(self);
    self->multitonKey = strdup(key);
    return self;
}

void DeleteView(char *key) {
    pthread_mutex_lock(&view_mutex);
    RemoveViewMap(key);
    pthread_mutex_unlock(&view_mutex);
}

View *getViewInstance(char *key, View *(*factory)(char *)) {
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