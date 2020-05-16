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
 * @param self
 * @param mediatorName
 * @param viewComponent
 */
void InitMediator(Mediator *self, const char *mediatorName, void *viewComponent) {
    self->notifier = NewNotifier();
    self->mediatorName = mediatorName != NULL ? mediatorName : MEDIATOR_NAME;
    self->viewComponent = viewComponent;
    self->getMediatorName = getMediatorName;
    self->setViewComponent = setViewComponent;
    self->getViewComponent = getViewComponent;
    self->listNotificationInterests = listNotificationInterests;
    self->handleNotification = handleNotification;
    self->onRegister = onRegister;
    self->onRemove = onRemove;
}

/**
 * Constructor
 *
 * @param mediatorName
 * @param viewComponent
 */
Mediator *NewMediator(const char *mediatorName, void *viewComponent) {
    Mediator *self = malloc(sizeof(Mediator));
    if (self == NULL) goto exception;
    InitMediator(self, mediatorName, viewComponent);
    return self;

    exception:
        fprintf(stderr, "Mediator allocation failed.\n");
        return NULL;
}

/**
 * Destructor
 *
 * @param self
 */
void DeleteMediator(Mediator *self) {
    DeleteNotifier(self->notifier);
    free(self);
    self = NULL;
}
