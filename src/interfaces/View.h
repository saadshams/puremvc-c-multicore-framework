#ifndef PUREMVC_VIEW_H
#define PUREMVC_VIEW_H

#include "Mediator.h"
#include "Observer.h"
#include <stdbool.h>

typedef struct MediatorMap MediatorMap;

struct MediatorMap {
    char *name;
    Mediator *mediator;
    MediatorMap *next;
};

typedef struct ObserverMap ObserverMap;

typedef struct ObserverNode ObserverNode;

struct ObserverMap {
    char *name;
    ObserverNode *observers;
    ObserverMap *next;
};

struct ObserverNode {
    Observer *observer;
    ObserverNode *next;
};

typedef struct View View;

typedef struct ViewMap ViewMap;

struct ViewMap {
    char *name;
    View *view;
    ViewMap *next;
};

struct View {
    char *multitonKey;
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

View *NewView(char *key);

void DeleteView(char *key);

View *getViewInstance(char *key, View *(factory)(char *));

#endif //PUREMVC_VIEW_H
