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
    if (type != NULL && copy == NULL) {
        fprintf(stderr, "[PureMVC::Notification::%s] Error: Failed to duplicate type string (name='%s'), (type='%s').\n",
            __func__, this->name ? this->name : "NULL", this->type ? this->type : "NULL");
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

    const size_t len = (this->name ? strlen(this->name) : 0) + (this->type ? strlen(this->type) : 0) + 32; // 32 or 64 bit pointer + extra
    char *msg = malloc(len * sizeof(char));
    if (msg == NULL) {
        fprintf(stderr, "[PureMVC::Notification::%s] Error: Failed to allocate string for Notification (name='%s', type='%s').\n",
              __func__,this->name ? this->name : "NULL", this->type ? this->type : "NULL");
        return NULL;
    }

    snprintf(msg, len, "%s : %s [body=%p]", this->name, this->type, this->body);
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
        fprintf(stderr, "[PureMVC::Notification::%s] Error: Failed to allocate Notification with name '%s'.\n", __func__, name);
        return NULL;
    }
    memset(notification, 0, sizeof(struct Notification));

    if (name != NULL) {
        notification->name = strdup(name);
        if (notification->name == NULL) {
            fprintf(stderr, "[PureMVC::Notification::%s] Error: strdup failed for name '%s'.\n", __func__, name);
            free(notification);
            return NULL;
        }
    }

    notification->body = body;

    if (type != NULL) {
        notification->type = strdup(type);
        if (notification->type == NULL) {
            fprintf(stderr, "[PureMVC::Notification::%s] Error: strdup failed for type '%s'.\n", __func__, type);
            free((void *) notification->name);
            free(notification);
            return NULL;
        }
    }

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
