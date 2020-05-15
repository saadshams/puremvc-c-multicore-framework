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

Notification *NewNotification(const char *name, void *body, char *type) {
    Notification *self = malloc(sizeof(Notification));
    if (self == NULL) goto exception;
    InitNotification(self, name, body, type);
    return self;

    exception:
        fprintf(stderr, "Notification allocation failed.\n");
        return NULL;
}

void DeleteNotification(Notification *self) {
    free(self);
    self = NULL;
}
