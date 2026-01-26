/**
* @file SimpleCommand.c
* @internal
* @brief SimpleCommand Implementation
*
* @author Saad Shams <saad.shams@puremvc.org>
* @copyright BSD 3-Clause License
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "SimpleCommand.h"

static void execute(const struct ICommand *self, struct INotification *notification, const char **error) {

}

static struct SimpleCommand *init(struct SimpleCommand *command) {
    if (command == NULL) return NULL;
    command->base.execute = execute;
    return command;
}

static struct SimpleCommand *alloc(const char **error) {
    struct SimpleCommand *command = malloc(sizeof(struct SimpleCommand));
    if (command == NULL) return *error = "[PureMVC::SimpleCommand::alloc] Error: Failed to allocate SimpleCommand", NULL;

    memset(command, 0, sizeof(struct SimpleCommand));

    command->base.notifier = puremvc_notifier_new(error);
    if (command->base.notifier == NULL) return free(command), NULL;

    return command;
}

struct ICommand *puremvc_simple_command_new(const char **error) {
    return (struct ICommand *) init(alloc(error));
}

void puremvc_simple_command_free(struct ICommand **command) {
    if (command == NULL || *command == NULL) return;
    struct SimpleCommand *this = (struct SimpleCommand *) *command;

    puremvc_notifier_free(&this->base.notifier);

    free(this);
    *command = NULL;
}
