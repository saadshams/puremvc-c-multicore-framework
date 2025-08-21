#include <stdlib.h>

#include "puremvc/IMediator.h"
#include "ViewTestMediator.h"

static char **listNotificationInterests(const struct IMediator *self) {
    static const char* interests[] = {"ABC", "DEF", "GHI", NULL};
    return self->allocNotificationInterests(self, interests);
}

struct IMediator *test_mediator_new(struct ViewComponent *component) {
    struct IMediator *mediator = puremvc_mediator_new(ViewTestMediator_NAME, component);
    mediator->listNotificationInterests = listNotificationInterests;
    return mediator;
}
