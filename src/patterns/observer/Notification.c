#include "interfaces/Notification.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char *getName(Notification *self) {
    return self->name;
}

static void setBody(Notification *self, void *body) {
    self->body = body;
}

static void *getBody(Notification *self) {
    return self->body;
}

static void setType(Notification *self, char *type) {
    self->type = type;
}

static char *getType(Notification *self) {
    return self->type;
}

static const char * toString(Notification *self) {
    char *msg = malloc(strlen(self->name) + (self->type != NULL ? strlen(self->type) : 0));
    strcat(msg, self->name);
    if (self->type) strcat(msg, self->type);
    return msg;
}

Notification *NewNotification(const char *name, void *body, char *type) {
    Notification *notification = malloc(sizeof(Notification));
    if (notification == NULL) goto exception;
    InitNotification(notification, name, body, type);
    return notification;

    exception:
        fprintf(stderr, "Notification allocation failed.\n");
        return NULL;
}

void InitNotification(Notification *notification, const char *name, void *body, char *type) {
    notification->name = name;
    notification->body = body;
    notification->type = type != NULL ? type : NULL;
    notification->getName = getName;
    notification->setBody = setBody;
    notification->getBody = getBody;
    notification->setType = setType;
    notification->getType = getType;
    notification->toString = toString;
}

void DeleteNotification(Notification *notification) {
    free(notification);
    notification = NULL;
}
