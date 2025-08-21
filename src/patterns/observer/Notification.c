#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "Notification.h"

static const char *getName(const struct INotification *self) {
    const struct Notification *this = (struct Notification *) self;
    return this->name;
}

static const void *getBody(const struct INotification *self) {
    const struct Notification *this = (struct Notification *) self;
    return this->body;
}

static void setBody(struct INotification *self, void *body) {
    struct Notification *this = (struct Notification *) self;
    free(this->body);
    this->body = body;
}

static const char *getType(const struct INotification *self) {
    const struct Notification *this = (struct Notification *) self;
    return this->type;
}

static void setType(struct INotification *self, const char *type) {
    struct Notification *this = (struct Notification *) self;
    char *copy = type ? strdup(type) : NULL;
    if (type && copy == NULL) {
        fprintf(stderr, "Notification: setType strdup failed.\n");
        return;
    }

    free(this->type);
    this->type = copy;
}

/**
 * Returns a newly allocated string. Caller must free the result.
 */
static const char *toString(const struct INotification *self) {
    const struct Notification *this = (struct Notification *) self;

    const size_t len = strlen(this->name) + (this->type ? strlen(this->type) : 0) + 1;
    char *msg = malloc(len * sizeof(char));
    // char *msg = malloc((strlen(self->name) + (self->type != NULL ? strlen(self->type) : 0) + 1) * sizeof(char));
    if (msg == NULL) {
        fprintf(stderr, "Notification: toString allocation failed.\n");
        return NULL;
    }

    msg[0] = '\0'; // Initialize to empty string before strcat
    strcat(msg, this->name);
    if (this->type)
        strcat(msg, this->type);
    return msg;
}

static struct Notification *init(struct Notification *notification) {
    if (notification == NULL) return NULL;
    notification->base.getName = getName;
    notification->base.getBody = getBody;
    notification->base.setBody = setBody;
    notification->base.getType = getType;
    notification->base.setType = setType;
    notification->base.toString = toString;
    return notification;
}

static struct Notification *alloc(const char *name, void *body, const char *type) {
    struct Notification *notification = malloc(sizeof(struct Notification));
    if (notification == NULL) {
        fprintf(stderr, "Notification allocation failed.\n");
        return NULL;
    }
    memset(notification, 0, sizeof(struct Notification));

    notification->name = name != NULL ? strdup(name) : NULL;
    notification->body = body;
    notification->type = type != NULL ? strdup(type) : NULL;
    return notification;
}

struct INotification *puremvc_notification_new(const char *name, void *body, const char *type) {
    return (struct INotification *) init(alloc(name, body, type));
}

void puremvc_notification_free(struct INotification **notification) {
    if (notification == NULL || *notification == NULL) return;
    struct Notification *this = (struct Notification *) *notification;

    free((void *) this->name);
    free(this->type);

    free(this);
    *notification = NULL;
}
