#pragma once

struct INotification {
    const char *(*getName)(const struct INotification *self);

    const void *(*getBody)(const struct INotification *self);
    void (*setBody)(struct INotification *self, void *body);

    const char *(*getType)(const struct INotification *self);
    void (*setType)(struct INotification *self, const char *type);
    
    const char *(*toString)(const struct INotification *self);
};

struct INotification *puremvc_notification_new(const char *name, void *body, const char *type);
void puremvc_notification_free(struct INotification **notification);
