#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "SimpleCommand.h"

static void execute(const struct ICommand *self, struct INotification *notification) {

}

static struct SimpleCommand *init(struct SimpleCommand *command) {
    if (command == NULL) return NULL;
    command->base.execute = execute;
    return command;
}

static struct SimpleCommand *alloc() {
    struct SimpleCommand *command = malloc(sizeof(struct SimpleCommand));
    if (command == NULL) {
        fprintf(stderr, "SimpleCommand allocation failed.\n");
        return NULL;
    }

    memset(command, 0, sizeof(struct SimpleCommand));
    command->base.notifier = puremvc_notifier_new();
    return command;
}

struct ICommand *puremvc_simple_command_new() {
    return (struct ICommand *) init(alloc());
}

void puremvc_simple_command_free(struct ICommand **command) {
    if (command == NULL || *command == NULL) return;

    struct SimpleCommand *this = (struct SimpleCommand *)*command;
    puremvc_notifier_free(&this->base.notifier);
    free(this);

    *command = NULL;
}
