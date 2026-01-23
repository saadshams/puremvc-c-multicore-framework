#include <stdlib.h>

#include "ViewTestMediator.h"

static char **listNotificationInterests(const struct IMediator *self, const char **error) {
    static const char* interests[] = {"ABC", "DEF", "GHI", NULL};
    return self->allocNotificationInterests(self, interests, error);
}

struct IMediator *test_mediator_new(struct ViewComponent *component) {
    const char *error = NULL;
    struct IMediator *mediator = puremvc_mediator_new(ViewTestMediator_NAME, component, &error);
    mediator->listNotificationInterests = listNotificationInterests;
    return mediator;
}
