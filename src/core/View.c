#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <collection/IArray.h>
#include <collection/IDictionary.h>

#include "View.h"

// The Multiton View instanceMap.
static struct IDictionary *instanceMap;

// mutex for instanceMap
static MutexOnce token = MUTEX_ONCE_INIT;
static Mutex mutex;

static void initializeView(struct IView *self) {

}

static void registerObserver(const struct IView *self, const char *notificationName, const struct IObserver *observer) {
    struct View *this = (struct View *) self;
    mutex_lock(&this->observerMapMutex);
    if (this->observerMap->containsKey(this->observerMap, notificationName)) {
        struct IArray *observers = (struct IArray *) this->observerMap->get(this->observerMap, notificationName);
        observers->push(observers, observer);
    } else {
        struct IArray *observers = collection_array_new();
        observers->push(observers, observer);
        this->observerMap->put(this->observerMap, notificationName, observers);
    }
    mutex_unlock(&this->observerMapMutex);
}

static void notifyObserver(const void *element, int index, const void *notification) {
    const struct IObserver *observer = element;
    observer->notifyObserver(observer, (void *)notification);
}

static void notifyObservers(const struct IView *self, const struct INotification *notification) {
    struct View *this = (struct View *) self;
    mutex_lock_shared(&this->observerMapMutex);

    if (this->observerMap->containsKey(this->observerMap, notification->getName(notification))) {
        // Get a reference to the clone list for this notification name
        const struct IArray *observers = (struct IArray *) this->observerMap->get(this->observerMap, notification->getName(notification));

        // Copy observers from reference array to working array,
        // since the reference array may change during the notification loop
        struct IArray *copy = observers->clone(observers);
        mutex_unlock(&this->observerMapMutex);

        copy->forEach(copy, notifyObserver, notification);
        copy->clear(copy, NULL);
        collection_array_free(&copy);
        return;
    }
    mutex_unlock(&this->observerMapMutex);
}

static bool compareNotifyContext(const void *element, const void *notifyContext) {
    const struct IObserver *observer = (struct IObserver *) element;
    return observer->compareNotifyContext(observer, notifyContext);
}

static void removeObserver(const struct IView *self, const char *notificationName, const void *notifyContext) {
    struct View *this = (struct View *) self;
    mutex_lock(&this->observerMapMutex);

    struct IArray *observers = (struct IArray *) this->observerMap->get(this->observerMap, notificationName);

    struct IObserver *observer = (struct IObserver *) observers->find(observers, compareNotifyContext, notifyContext);
    if (observer != NULL) {
        observers->removeItem(observers, observer);
        puremvc_observer_free(&observer);
    }

    if (observers->size(observers) == 0) {
        this->observerMap->removeItem(this->observerMap, notificationName);
    }

    mutex_unlock(&this->observerMapMutex);
}

static void registerMediator(const struct IView *self, struct IMediator *mediator) {
    struct View *this = (struct View *) self;
    mutex_lock(&this->mediatorMapMutex);

    if (this->mediatorMap->containsKey(this->mediatorMap, mediator->getName(mediator))) {
        mutex_unlock(&this->mediatorMapMutex);
        return;
    }

    mediator->notifier->initializeNotifier(mediator->notifier, this->multitonKey);
    this->mediatorMap->put(this->mediatorMap, mediator->getName(mediator), mediator);

    char **interests = mediator->listNotificationInterests(mediator);
    for(char **cursor = interests; *cursor; cursor++) {
        const struct IObserver *observer = puremvc_observer_new((const void (*)(const void *, struct INotification *)) mediator->handleNotification, mediator);
        self->registerObserver(self, *cursor, observer);
    }
    mediator->freeNotificationInterests(mediator, interests);

    mediator->onRegister(mediator);
    mutex_unlock(&this->mediatorMapMutex);
}

static struct IMediator *retrieveMediator(const struct IView *self, const char *mediatorName) {
    struct View *this = (struct View *) self;
    mutex_lock_shared(&this->mediatorMapMutex);
    struct IMediator *mediator = (struct IMediator *) this->mediatorMap->get(this->mediatorMap, mediatorName);
    mutex_unlock(&this->mediatorMapMutex);
    return mediator;
}

static bool hasMediator(const struct IView *self, const char *mediatorName) {
    struct View *this = (struct View *) self;
    mutex_lock_shared(&this->mediatorMapMutex);
    const bool exists = this->mediatorMap->containsKey(this->mediatorMap, mediatorName);
    mutex_unlock(&this->mediatorMapMutex);
    return exists;
}

static struct IMediator *removeMediator(const struct IView *self, const char *mediatorName) {
    struct View *this = (struct View *) self;
    mutex_lock(&this->mediatorMapMutex);
    struct IMediator *mediator = this->mediatorMap->removeItem(this->mediatorMap, mediatorName);
    if (mediator != NULL) {
        char **interests = mediator->listNotificationInterests(mediator);
        for (char **cursor = interests; *cursor; cursor++) {
            self->removeObserver(self, *cursor, mediator);
        }
        mediator->freeNotificationInterests(mediator, interests);
        mediator->onRemove(mediator);
    }
    mutex_unlock(&this->mediatorMapMutex);
    return mediator;
}

static struct View *init(struct View *view) {
    if (view == NULL) return NULL;
    mutex_init(&view->mediatorMapMutex);
    mutex_init(&view->observerMapMutex);
    view->base.initializeView = initializeView;
    view->base.registerObserver = registerObserver;
    view->base.notifyObservers = notifyObservers;
    view->base.removeObserver = removeObserver;
    view->base.registerMediator = registerMediator;
    view->base.retrieveMediator = retrieveMediator;
    view->base.hasMediator = hasMediator;
    view->base.removeMediator = removeMediator;
    return view;
}

static struct View *alloc(const char *key) {
    assert(instanceMap->get(instanceMap, key) == NULL);

    struct View *view = malloc(sizeof(struct View));
    if (view == NULL) {
        fprintf(stderr, "View allocation failed.\n");
        return NULL;
    }

    memset(view, 0, sizeof(struct View));

    view->multitonKey = strdup(key);
    view->mediatorMap = collection_dictionary_new();
    view->observerMap = collection_dictionary_new();
    return view;
}

struct IView *puremvc_view_new(const char *key) {
    return (struct IView *) init(alloc(key));
}

void puremvc_view_free(struct IView **view) {
    if (view == NULL || *view == NULL) return;

    struct View *this = (struct View *) *view;
    free((void *)this->multitonKey);
    this->mediatorMap->clear(this->mediatorMap, free);
    collection_dictionary_free(&this->mediatorMap);
    this->observerMap->clear(this->observerMap, free);
    collection_dictionary_free(&this->observerMap);
    mutex_destroy(&this->mediatorMapMutex);
    mutex_destroy(&this->observerMapMutex);
    free(this);

    *view = NULL;
}

static void dispatchOnce() {
    mutex_init(&mutex);
}

struct IView *puremvc_view_getInstance(const char *key, struct IView *(*factory)(const char *)) {
    mutex_once(&token, dispatchOnce);
    mutex_lock(&mutex);

    if (instanceMap == NULL) instanceMap = collection_dictionary_new();

    struct IView *instance = (struct IView *) instanceMap->get(instanceMap, key);
    if (instance == NULL) {
        instance = factory(key);
        instanceMap->put(instanceMap, key, instance);
        instance->initializeView(instance);
    }
    mutex_unlock(&mutex);
    return instance;
}

void puremvc_view_removeView(const char *key) {
    mutex_lock(&mutex);
    struct IView *view = instanceMap->removeItem(instanceMap, key);
    if (view != NULL) puremvc_view_free(&view);
    mutex_unlock(&mutex);
}
