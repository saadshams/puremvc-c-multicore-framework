#include "interfaces/Mediator.h"
#include "interfaces/Notifier.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Get mediator name
 *
 * @param self
 * @return
 */
static const char *getMediatorName(Mediator *self) {
    return self->mediatorName;
}

/**
 * Set the view component
 *
 * @param self
 * @param viewComponent
 */
static void setViewComponent(Mediator *self, void *viewComponent) {
    self->viewComponent = viewComponent;
}

/**
 * Get the view component
 *
 * @param self
 * @return
 */
static void *getViewComponent(Mediator *self) {
    return self->viewComponent;
}

/**
 * <P>List the <code>INotification</code> names this
 * <code>Mediator</code> is interested in being notified of.</P>
 *
 * @param self
 * @return Array the list of <code>INotification</code> names
 */
static const char * const *listNotificationInterests(Mediator *self) {
    static const char * const interests[] = {NULL};
    return interests;
}

/**
 * <P>Handle <code>INotification</code>s.</P>
 *
 * <P>Typically this will be handled in a switch statement,
 * with one 'case' entry per <code>INotification</code>
 * the <code>Mediator</code> is interested in.</P>
 */
static void handleNotification(Mediator *self, Notification *notification) {

}

/**
 * <P>Called by the View when the Mediator is registered</P>
 */
static void onRegister(Mediator *self) {

}

/**
 * <P>Called by the View when the Mediator is removed</P>
 */
static void onRemove(Mediator *self) {

}

/**
 * Initializer
 *
 * @param mediator
 * @param mediatorName
 * @param viewComponent
 */
void InitMediator(Mediator *mediator, const char *mediatorName, void *viewComponent) {
    mediator->notifier = NewNotifier();
    mediator->mediatorName = mediatorName != NULL ? mediatorName : MEDIATOR_NAME;
    mediator->viewComponent = viewComponent;
    mediator->getMediatorName = getMediatorName;
    mediator->setViewComponent = setViewComponent;
    mediator->getViewComponent = getViewComponent;
    mediator->listNotificationInterests = listNotificationInterests;
    mediator->handleNotification = handleNotification;
    mediator->onRegister = onRegister;
    mediator->onRemove = onRemove;
}

/**
 * Constructor
 *
 * @param mediatorName
 * @param viewComponent
 */
Mediator *NewMediator(const char *mediatorName, void *viewComponent) {
    Mediator *mediator = malloc(sizeof(Mediator));
    if (mediator == NULL) goto exception;
    InitMediator(mediator, mediatorName, viewComponent);
    return mediator;

    exception:
        fprintf(stderr, "Mediator allocation failed.\n");
        return NULL;
}

/**
 * Destructor
 *
 * @param mediator
 */
void DeleteMediator(Mediator *mediator) {
    DeleteNotifier(mediator->notifier);
    free(mediator);
    mediator = NULL;
}
