#ifndef PUREMVC_VIEW_H
#define PUREMVC_VIEW_H

#include "Mediator.h"
#include "Observer.h"
#include <stdbool.h>

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

void InitView(View *self);

View *NewView(const char *key);

void DeleteView(const char *key);

View *getViewInstance(const char *key, View *(factory)(const char *));

#endif //PUREMVC_VIEW_H
