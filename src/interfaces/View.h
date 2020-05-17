#ifndef PUREMVC_VIEW_H
#define PUREMVC_VIEW_H

#include "Mediator.h"
#include "Observer.h"
#include <stdbool.h>

/**
 * <P>A Multiton <code>View</code> implementation.</P>
 *
 * <P>In PureMVC, the <code>View</code> class assumes these responsibilities:</P>
 *
 * <UL>
 * <LI>Maintain a cache of <code>Mediator</code> instances.</LI>
 * <LI>Provide methods for registering, retrieving, and removing <code>Mediators</code>.</LI>
 * <LI>Notifiying <code>Mediators</code> when they are registered or removed.</LI>
 * <LI>Managing the observer lists for each <code>Notification</code> in the application.</LI>
 * <LI>Providing a method for attaching <code>Observers</code> to an <code>Notification</code>'s observer list.</LI>
 * <LI>Providing a method for broadcasting an <code>Notification</code>.</LI>
 * <LI>Notifying the <code>Observers</code> of a given <code>Notification</code> when it broadcast.</LI>
 * </UL>
 *
 * @see Mediator
 * @see Observer
 * @see Notification
 */
typedef struct MediatorMap MediatorMap;

struct MediatorMap {
    const char *name;
    Mediator *mediator;
    MediatorMap *next;
};

typedef struct ObserverNode ObserverNode;

struct ObserverNode {
    Observer *observer;
    ObserverNode *next;
};

typedef struct ObserverMap ObserverMap;

struct ObserverMap {
    const char *name;
    ObserverNode *observers;
    ObserverMap *next;
};

typedef struct View View;

struct View {
    const char *multitonKey;
    MediatorMap *mediatorMap;
    ObserverMap *observerMap;
    void (*initializeView)(View *self);
    void (*registerObserver)(View *self, const char *notificationName, Observer *observer);
    void (*notifyObservers)(View *self, Notification *notification);
    void (*removeObserver)(View *self, const char *notificationName, void *notifyContext);
    void (*registerMediator)(View *self, Mediator *mediator);
    Mediator *(*retrieveMediator)(View *self, const char *mediatorName);
    bool (*hasMediator)(View *self, const char *mediatorName);
    Mediator *(*removeMediator)(View *self, const char *mediatorName);
};

View *getViewInstance(const char *key, View *(factory)(const char *));

View *NewView(const char *key);

void InitView(View *view);

void RemoveView(const char *key);

#endif //PUREMVC_VIEW_H
