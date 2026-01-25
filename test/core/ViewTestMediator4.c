#include <stdlib.h>

#include "ViewTestMediator4.h"

static const char **listNotificationInterests(const struct IMediator *self, const char **error) {
    static const char* interests[] = {NOTE4, NULL};
    return interests;
}

static void onRegister(struct IMediator *self) {
    struct ViewTest *viewTest = self->getComponent(self);
    viewTest->onRegisterCalled = true;
}

static void onRemove(struct IMediator *self) {
    struct ViewTest *viewTest = self->getComponent(self);
    viewTest->onRemoveCalled = true;
}

struct IMediator *test_mediator4_new(struct ViewTest *component) {
    const char *error = NULL;
    struct IMediator *mediator = puremvc_mediator_new(ViewTestMediator4_NAME, component, &error);
    mediator->listNotificationInterests = listNotificationInterests;
    mediator->onRegister = onRegister;
    mediator->onRemove = onRemove;
    return mediator;
}
