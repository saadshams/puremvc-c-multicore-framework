#ifndef PUREMVC_SIMPLECOMMAND_H
#define PUREMVC_SIMPLECOMMAND_H

#include "Notification.h"

struct Notifier;

typedef struct SimpleCommand SimpleCommand;

/**
 * <P>A base <code>Command</code> implementation.</P>
 *
 * <P>Your subclass should override the <code>execute</code>
 * method where your business logic will handle the <code>Notification</code>.</P>
 *
 * @see Controller
 * @see Notification
 * @see MacroCommand
 */
struct SimpleCommand {

    struct Notifier *notifier;

    /**
     * <P>Fulfill the use-case initiated by the given <code>Notification</code>.</P>
     *
     * <P>In the Command Pattern, an application use-case typically
     * begins with some user action, which results in an <code>Notification</code> being broadcast, which
     * is handled by business logic in the <code>execute</code> method of an
     * <code>Command</code>.</P>
     *
     * @param self SimpleCommand
     * @param notification Notification
     */
    void (*execute)(SimpleCommand *self, Notification *notification);
};

struct $SimpleCommand {
    /** Constructor */
    SimpleCommand *(*new)(void);

    /** Initializer */
    void (*init)(SimpleCommand *simpleCommand);

    /** Destructor */
    void (*delete)(SimpleCommand *simpleCommand);
};

const struct $SimpleCommand $SimpleCommand;


#endif //PUREMVC_SIMPLECOMMAND_H
