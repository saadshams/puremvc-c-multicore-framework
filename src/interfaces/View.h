#ifndef PUREMVC_VIEW_H
#define PUREMVC_VIEW_H

#include "Mediator.h"
#include "Observer.h"
#include <stdbool.h>

/**
 * <P>A Multiton <code>IView</code> implementation.</P>
 *
 * <P>In PureMVC, the <code>View</code> class assumes these responsibilities:</P>
 *
 * <UL>
 * <LI>Maintain a cache of <code>IMediator</code> instances.</LI>
 * <LI>Provide methods for registering, retrieving, and removing <code>IMediators</code>.</LI>
 * <LI>Notifiying <code>IMediators</code> when they are registered or removed.</LI>
 * <LI>Managing the observer lists for each <code>INotification</code> in the application.</LI>
 * <LI>Providing a method for attaching <code>IObservers</code> to an <code>INotification</code>'s observer list.</LI>
 * <LI>Providing a method for broadcasting an <code>INotification</code>.</LI>
 * <LI>Notifying the <code>IObservers</code> of a given <code>INotification</code> when it broadcast.</LI>
 * </UL>
 *
 * @see org.puremvc.c.multicore.patterns.mediator.Mediator Mediator
 * @see org.puremvc.c.multicore.patterns.observer.Observer Observer
 * @see org.puremvc.c.multicore.patterns.observer.Notification Notification
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
