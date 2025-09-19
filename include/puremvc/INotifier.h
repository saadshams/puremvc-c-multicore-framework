#pragma once

struct IFacade; // Forward-declare IFacade to avoid including it and break reference cycle

struct INotifier {
    struct IFacade *(*getFacade)(const struct INotifier *self);
    const char *(*getKey)(const struct INotifier *self);

    void (*initializeNotifier)(struct INotifier *self, const char *key);
    void (*sendNotification)(const struct INotifier *self, const char *notificationName, void *body, const char *type);
};

struct INotifier *puremvc_notifier_new();
void puremvc_notifier_free(struct INotifier **notifier);
