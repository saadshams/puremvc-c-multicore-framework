#include "interfaces/Notification.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Get the name of the <code>Notification</code> instance.
 *
 * @param self
 * @return the name of the <code>Notification</code> instance.
 */
static const char *getName(Notification *self) {
    return self->name;
}

/**
 * Set the body of the <code>Notification</code> instance.
 *
 * @param self
 */
static void setBody(Notification *self, void *body) {
    self->body = body;
}

/**
 * Get the body of the <code>Notification</code> instance.
 *
 * @param self
 * @return the body object.
 */
static void *getBody(Notification *self) {
    return self->body;
}

/**
 * Set the type of the <code>Notification</code> instance.
 *
 * @param self
 */
static void setType(Notification *self, char *type) {
    self->type = type;
}

/**
 * Get the type of the <code>Notification</code> instance.
 *
 * @param self
 * @return the type
 */
static char *getType(Notification *self) {
    return self->type;
}

/**
 * Initializer
 *
 * @param notification
 * @param name
 * @param body
 * @param type
 */
void InitNotification(Notification *notification, const char *name, void *body, char *type) {
    notification->name = name;
    notification->body = body;
    notification->type = type != NULL ? type : NULL;
    notification->getName = getName;
    notification->setBody = setBody;
    notification->getBody = getBody;
    notification->setType = setType;
    notification->getType = getType;
}

/**
 * Constructor
 *
 * @param name
 * @param body
 * @param type
 */
Notification *NewNotification(const char *name, void *body, char *type) {
    Notification *notification = malloc(sizeof(Notification));
    if (notification == NULL) goto exception;
    InitNotification(notification, name, body, type);
    return notification;

    exception:
        fprintf(stderr, "Notification allocation failed.\n");
        return NULL;
}

/**
 * Destructor
 *
 * @param notification
 */
void DeleteNotification(Notification *notification) {
    free(notification);
    notification = NULL;
}
