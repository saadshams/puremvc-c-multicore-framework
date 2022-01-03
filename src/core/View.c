#include "interfaces/List.h"
#include "interfaces/Notifier.h"
#include "interfaces/View.h"
#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// The Multiton View instanceMap.
static Map *instanceMap;

// mutex for instanceMap
static pthread_rwlock_t view_mutex;

// mutex for mediatorMap
static pthread_rwlock_t mediatorMap_mutex;

// mutex for observerMap
static pthread_rwlock_t observerMap_mutex;

static void initializeView(View *self) {

}

static void registerObserver(View *self, const char *notificationName, Observer *observer) {
    pthread_rwlock_wrlock(&observerMap_mutex);
    if ($Map.containsKey(&self->observerMap, notificationName)) {
        List *observers = (List *) $Map.get(&self->observerMap, notificationName);
        $List.addLast(&observers, observer);
    } else {
        List *observers = NULL;
        $List.addLast(&observers, observer);
        $Map.put(&self->observerMap, notificationName, observers);
    }
    pthread_rwlock_unlock(&observerMap_mutex);
}

static void notifyObservers(View *self, Notification *notification) {
    pthread_rwlock_rdlock(&observerMap_mutex);

    if ($Map.containsKey(&self->observerMap, notification->name)) {
        // Get a reference to the clone list for this notification name
        List *observers = (List *) $Map.get(&self->observerMap, notification->name);

        // Copy observers from reference array to working array,
        // since the reference array may change during the notification loop
        List *cursor = $List.clone(&observers);

        // Notify Observers from the working array
        while (cursor) {
            Observer *observer = (Observer *) cursor->data;
            observer->notifyObserver(observer, notification);
            cursor = cursor->next;
        }
    }
    pthread_rwlock_unlock(&observerMap_mutex);
}

static void removeObserver(View *self, const char *notificationName, void *notifyContext) {
    pthread_rwlock_wrlock(&observerMap_mutex);

    List *cursor = (List *) $Map.get(&self->observerMap, notificationName);
    while (cursor) {
        Observer *observer = (Observer *) cursor->data;
        if (observer->compareNotifyContext(observer, notifyContext)) {
            $List.remove(&cursor, observer);
            break;
        }
        cursor = cursor->next;
    }

    if ($List.size(&cursor) == 0) {
        $Map.remove(&self->observerMap, notificationName);
    }

    pthread_rwlock_unlock(&observerMap_mutex);
}

static void registerMediator(View *self, Mediator *mediator) {
    pthread_rwlock_wrlock(&mediatorMap_mutex);

    // do not allow re-registration (you must to removeMediator first)
    if ($Map.containsKey(&self->mediatorMap, mediator->mediatorName)) return;

    mediator->notifier->initializeNotifier(mediator->notifier, self->multitonKey);

    $Map.put(&self->mediatorMap, mediator->mediatorName, mediator);

    // Get Notification interests, if any.
    const char *const *interests = mediator->listNotificationInterests(mediator);

    // Register Mediator as an observer for each notification of interests
    while (*interests) {
        // Create Observer referencing this mediator's handleNotification method
        Observer *observer = $Observer.new((void (*)(void *, Notification *)) mediator->handleNotification, mediator);

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
    Mediator *mediator = (Mediator *) $Map.get(&self->mediatorMap, mediatorName);
    pthread_rwlock_unlock(&mediatorMap_mutex);
    return mediator;
}

static bool hasMediator(View *self, const char *mediatorName) {
    pthread_rwlock_rdlock(&mediatorMap_mutex);
    bool exists = $Map.containsKey(&self->mediatorMap, mediatorName);
    pthread_rwlock_unlock(&mediatorMap_mutex);
    return exists;
}

static Mediator *removeMediator(View *self, const char *mediatorName) {
    pthread_rwlock_wrlock(&mediatorMap_mutex);
    Mediator *mediator = (Mediator *) $Map.remove(&self->mediatorMap, mediatorName);
    if (mediator) {
        // for every notification this mediator is interested in...
        const char *const *interests = mediator->listNotificationInterests(mediator);
        while (*interests) {
            // remove the observer linking the mediator
            // to the notification interest
            self->removeObserver(self, *interests, mediator);
            interests++;
        }

        // alert the mediator that it has been removed
        mediator->onRemove(mediator);
        return mediator;
    }
    pthread_rwlock_unlock(&mediatorMap_mutex);
    return NULL;
}

static void init(View *view) {
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

static View *new(const char *key) {
    assert($Map.get(&instanceMap, key) == NULL);

    View *view = malloc(sizeof(View));
    if (view == NULL) goto exception;
    init(view);
    view->multitonKey = key;
    return view;

    exception:
    fprintf(stderr, "View allocation failed.\n");
    return NULL;
}

static void removeView(const char *key) {
    pthread_rwlock_wrlock(&view_mutex);
    View *view = (View *) $Map.remove(&instanceMap, key);
    if (view != NULL) free(view);
    pthread_rwlock_unlock(&view_mutex);
}

static View *getInstance(const char *key, View *(*factory)(const char *)) {
    pthread_rwlock_wrlock(&view_mutex);
    View *instance = (View *) $Map.get(&instanceMap, key);
    if (instance == NULL) {
        instance = (View *) $Map.put(&instanceMap, key, factory(key));
        instance->initializeView(instance);
    }
    pthread_rwlock_unlock(&view_mutex);
    return instance;
}

const struct $View $View = {.new = new, .init = init, .getInstance = getInstance, .removeView = removeView};
