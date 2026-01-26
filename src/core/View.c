/**
* @file View.c
* @internal
* @brief View Implementation
*
* @author Saad Shams <saad.shams@puremvc.org>
* @copyright BSD 3-Clause License
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <collection/IArray.h>
#include <collection/IDictionary.h>

#include "View.h"

// The Multiton View instanceMap.
static struct IDictionary *instanceMap = NULL;

// mutex for instanceMap
static MutexOnce token = MUTEX_ONCE_INIT;
static Mutex mutex;

static void initializeView(struct IView *self, const char **error) {

}

static void registerObserver(const struct IView *self, const char *notificationName, const struct IObserver *observer, const char **error) {
    struct View *this = (struct View *) self;

    mutex_lock(&this->observerMapMutex);
    if (this->observerMap->containsKey(this->observerMap, notificationName)) {
        struct IArray *observers = (struct IArray *) this->observerMap->get(this->observerMap, notificationName);
        observers->push(observers, observer, error);
        if (*error != NULL) return mutex_unlock(&this->observerMapMutex), (void)0;
    } else {
        struct IArray *observers = collection_array_new(error);
        if (*error != NULL) return mutex_unlock(&this->observerMapMutex), (void)0;

        observers->push(observers, observer, error);
        if (*error != NULL) return collection_array_free(&observers), mutex_unlock(&this->observerMapMutex), (void)0;

        this->observerMap->put(this->observerMap, notificationName, observers, error);
        if (*error != NULL) return observers->removeItem(observers, observer), collection_array_free(&observers),
            mutex_unlock(&this->observerMapMutex), (void)0;
    }
    mutex_unlock(&this->observerMapMutex);
}

static void notifyObserver(const void *element, const void *notification, const char **error) {
    const struct IObserver *observer = element;
    observer->notifyObserver(observer, (void *)notification, error);
}

static void notifyObservers(const struct IView *self, const struct INotification *notification, const char **error) {
    struct View *this = (struct View *) self;
    struct IArray *copy = NULL;

    mutex_lock_shared(&this->observerMapMutex);
    if (this->observerMap->containsKey(this->observerMap, notification->getName(notification))) {
        // Get a reference to the clone list for this notification name
        const struct IArray *observers = (struct IArray *) this->observerMap->get(this->observerMap, notification->getName(notification));

        // Copy observers from reference array to working array,
        // since the reference array may change during the notification loop
        copy = observers->clone(observers, error);
        if (*error != NULL) return mutex_unlock(&this->observerMapMutex), (void)0;
    }
    mutex_unlock(&this->observerMapMutex);

    if (copy == NULL) return;
    copy->forEach(copy, notifyObserver, notification, error);
    copy->clear(copy, NULL);
    collection_array_free(&copy);
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

static void registerMediator(const struct IView *self, struct IMediator *mediator, const char **error) {
    struct View *this = (struct View *) self;

    mutex_lock(&this->mediatorMapMutex);
    if (this->mediatorMap->containsKey(this->mediatorMap, mediator->getName(mediator)))
        return mutex_unlock(&this->mediatorMapMutex), (void)0;

    mediator->notifier->initializeNotifier(mediator->notifier, this->multitonKey, error);
    if (*error != NULL) return mutex_unlock(&this->mediatorMapMutex), (void)0;

    this->mediatorMap->put(this->mediatorMap, mediator->getName(mediator), mediator, error);
    if (*error != NULL) return mutex_unlock(&this->mediatorMapMutex), (void)0;
    mutex_unlock(&this->mediatorMapMutex);

    const char **interests = mediator->listNotificationInterests(mediator, error);
    if (*error != NULL)
        return mutex_lock(&this->mediatorMapMutex), this->mediatorMap->removeItem(this->mediatorMap, mediator->getName(mediator)),
            mutex_unlock(&this->mediatorMapMutex), (void)0;

    for (const char **interest = interests; *interest; interest++) {
        const struct IObserver *observer = puremvc_observer_new((const void (*)(const void *, struct INotification *)) mediator->handleNotification, mediator, error);
        if (*error != NULL) { // rollback
            for (const char **cursor = interests; cursor < interest; cursor++)
                self->removeObserver(self, *cursor, mediator);
            return mutex_lock(&this->mediatorMapMutex), this->mediatorMap->removeItem(this->mediatorMap, mediator->getName(mediator)),
                mutex_unlock(&this->mediatorMapMutex), (void)0;
        }

        self->registerObserver(self, *interest, observer, error);
        if (*error != NULL) { // rollback
            puremvc_observer_free((struct IObserver **)&observer);
            for (const char **cursor = interests; cursor < interest; cursor++)
                self->removeObserver(self, *cursor, mediator);
            return mutex_lock(&this->mediatorMapMutex), this->mediatorMap->removeItem(this->mediatorMap, mediator->getName(mediator)),
                mutex_unlock(&this->mediatorMapMutex), (void)0;
        }
    }
    mediator->onRegister(mediator);
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

static struct IMediator *removeMediator(const struct IView *self, const char *mediatorName, const char **error) {
    struct View *this = (struct View *) self;

    mutex_lock(&this->mediatorMapMutex);
    struct IMediator *mediator = this->mediatorMap->removeItem(this->mediatorMap, mediatorName);
    mutex_unlock(&this->mediatorMapMutex);

    if (mediator != NULL) {
        const char **interests = mediator->listNotificationInterests(mediator, error);
        for (const char **cursor = interests; *cursor; cursor++) {
            self->removeObserver(self, *cursor, mediator);
        }
        mediator->onRemove(mediator);
    }

    return mediator;
}

static struct View *init(struct View *view) {
    if (view == NULL) return NULL;
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

static struct View *alloc(const char *key, const char **error) {
    struct View *view = malloc(sizeof(struct View));
    if (view == NULL) return *error = "[PureMVC::View::alloc] Error: Failed to allocate View", NULL;

    memset(view, 0, sizeof(struct View));

    view->multitonKey = strdup(key);
    if (view->multitonKey == NULL) return *error = "[PureMVC::View::alloc] Error: Failed to allocate View key (strdup)", free(view), NULL;

    mutex_init(&view->mediatorMapMutex);

    view->mediatorMap = collection_dictionary_new(error);
    if (*error != NULL) return mutex_destroy(&view->mediatorMapMutex), free((void *)view->multitonKey), free(view), NULL;

    mutex_init(&view->observerMapMutex);

    view->observerMap = collection_dictionary_new(error);
    if (*error != NULL) return mutex_destroy(&view->observerMapMutex), collection_dictionary_free(&view->mediatorMap),
        mutex_destroy(&view->mediatorMapMutex), free((void *)view->multitonKey), free(view), NULL;

    return view;
}

struct IView *puremvc_view_new(const char *key, const char **error) {
    if (key == NULL) return *error = "[PureMVC::View::new] Error: key must not be NULL", NULL;
    return (struct IView *) init(alloc(key, error));
}

static void mediator_free(void *value) {
    struct IMediator *mediator = value;
    puremvc_mediator_free(&mediator);
}

static void observer_free(void *value) {
    struct IObserver *observer = value;
    puremvc_observer_free(&observer);
}

void puremvc_view_free(struct IView **view) {
    if (view == NULL || *view == NULL) return;
    struct View *this = (struct View *) *view;

    free((void *) this->multitonKey);
    this->mediatorMap->clear(this->mediatorMap, mediator_free);
    collection_dictionary_free(&this->mediatorMap);
    this->observerMap->clear(this->observerMap, observer_free);
    collection_dictionary_free(&this->observerMap);

    mutex_destroy(&this->mediatorMapMutex);
    mutex_destroy(&this->observerMapMutex);

    free(this);
    *view = NULL;
}

static void dispatchOnce() {
    mutex_init(&mutex);
}

struct IView *puremvc_view_getInstance(const char *key, struct IView *(*factory)(const char *key, const char **error), const char **error) {
    if (key == NULL || factory == NULL) return *error = "[PureMVC::View::getInstance] Error: key or factory must not be NULL", NULL;
    mutex_once(&token, dispatchOnce);
    mutex_lock(&mutex);

    if (instanceMap == NULL) {
        instanceMap = collection_dictionary_new(error);
        if (*error != NULL) return mutex_unlock(&mutex), NULL;
    }

    struct IView *instance = (struct IView *) instanceMap->get(instanceMap, key);
    if (instance == NULL) {
        instance = factory(key, error);
        if (*error != NULL) return mutex_unlock(&mutex), NULL;

        instance->initializeView(instance, error);
        if (*error != NULL) return puremvc_view_free(&instance), mutex_unlock(&mutex), NULL;

        instanceMap->put(instanceMap, key, instance, error);
        if (*error != NULL) return puremvc_view_free(&instance), mutex_unlock(&mutex), NULL;
    }

    mutex_unlock(&mutex);
    return instance;
}

void puremvc_view_removeView(const char *key) {
    if (key == NULL) return;
    mutex_lock(&mutex);

    struct IView *view = instanceMap->removeItem(instanceMap, key);
    if (view != NULL) puremvc_view_free(&view);

    mutex_unlock(&mutex);
}
