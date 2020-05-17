#ifndef PUREMVC_MEDIATOR_H
#define PUREMVC_MEDIATOR_H

#include "Notification.h"

struct Notifier;

/**
 * <P>A base <code>Mediator</code> implementation.</P>
 *
 * @see org.puremvc.c.multicore.core.View View
 */
typedef struct Mediator Mediator;

struct Mediator {
    struct Notifier *notifier;
    const char *mediatorName;
    void *viewComponent;
    const char *(*getMediatorName)(Mediator *self);
    void (*setViewComponent)(Mediator *self, void *viewComponent);
    void *(*getViewComponent)(Mediator *self);
    const char * const *(*listNotificationInterests)(Mediator *self);
    void (*handleNotification)(Mediator *self, Notification *notification);
    void (*onRegister)(Mediator *self);
    void (*onRemove)(Mediator *self);
};

/**
 * <P>The name of the <code>Mediator</code>.</P>
 *
 * <P>Typically, a <code>Mediator</code> will be written to serve
 * one specific control or group controls and so,
 * will not have a need to be dynamically named.</P>
 */
#define MEDIATOR_NAME "Mediator"

/**
 * Initializer
 *
 * @param mediator
 * @param mediatorName
 * @param viewComponent
 */
void InitMediator(Mediator *mediator, const char *mediatorName, void *viewComponent);

/**
 * Constructor
 *
 * @param mediatorName
 * @param viewComponent
 */
Mediator *NewMediator(const char *mediatorName, void *viewComponent);

/**
 * Destructor
 *
 * @param mediator
 */
void DeleteMediator(Mediator *mediator);

#endif //PUREMVC_MEDIATOR_H
