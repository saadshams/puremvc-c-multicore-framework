#ifndef PUREMVC_NOTIFICATION_H
#define PUREMVC_NOTIFICATION_H

typedef struct Notification Notification;

struct Notification {
    const char *name;
    void *body;
    char *type;
    const char *(*getName)(Notification *self);
    void *(*getBody)(Notification *self);
    void (*setBody)(Notification *self, void *body);
    char *(*getType)(Notification *self);
    void (*setType)(Notification *self, char *type);
};

void InitNotification(Notification *self, const char *name, void *body, char *type);

Notification *NewNotification(const char *name, void *body, char *type);

void DeleteNotification(Notification *self);

#endif //PUREMVC_NOTIFICATION_H
