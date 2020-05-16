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
 * @param self
 * @param name
 * @param body
 * @param type
 */
void InitNotification(Notification *self, const char *name, void *body, char *type) {
    self->name = name;
    self->body = body;
    self->type = type != NULL ? type : NULL;
    self->getName = getName;
    self->setBody = setBody;
    self->getBody = getBody;
    self->setType = setType;
    self->getType = getType;
}

/**
 * Constructor
 *
 * @param name
 * @param body
 * @param type
 */
Notification *NewNotification(const char *name, void *body, char *type) {
    Notification *self = malloc(sizeof(Notification));
    if (self == NULL) goto exception;
    InitNotification(self, name, body, type);
    return self;

    exception:
        fprintf(stderr, "Notification allocation failed.\n");
        return NULL;
}

/**
 * Destructor
 *
 * @param self
 */
void DeleteNotification(Notification *self) {
    free(self);
    self = NULL;
}
