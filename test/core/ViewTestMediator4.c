#pragma once

#include <stdlib.h>

#include "puremvc/puremvc.h"
#include "ViewTestComponent.c"

#define ViewTestMediator4_NAME "Mediator4"

static char **listNotificationInterests4(const struct IMediator *self) {
    static const char* interests[] = {NOTE4, NULL};
    return self->allocNotificationInterests(self, interests);
}

static void onRegister4(struct IMediator *self) {
    struct ViewTest *viewTest = self->getComponent(self);
    viewTest->onRegisterCalled = true;
}

static void onRemove4(struct IMediator *self) {
    struct ViewTest *viewTest = self->getComponent(self);
    viewTest->onRemoveCalled = true;
}

struct IMediator *test_mediator4_new(struct ViewTest *component) {
    struct IMediator *mediator = puremvc_mediator_new(ViewTestMediator4_NAME, component);
    mediator->listNotificationInterests = listNotificationInterests4;
    mediator->onRegister = onRegister4;
    mediator->onRemove = onRemove4;
    return mediator;
}
