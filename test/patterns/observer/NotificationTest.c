#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include "puremvc/INotification.h"

/**
 * Tests setting the name and body using the Notification class Constructor.
 */
void testConstructor() {
    // Create a new Notification using the Constructor to set the note name and body
    struct INotification *notification = puremvc_notification_new("TestNote", NULL, NULL);

    // test assertions
    assert(strcmp(notification->getName(notification), "TestNote") == 0);
    assert(notification->getBody(notification) == NULL);
    assert(notification->getType(notification) == NULL);

    puremvc_notification_free(&notification);
    assert(notification == NULL);
}

/**
 * Tests setting and getting the name using Notification class accessor methods.
 */
void testNameAccessors() {
    // Create a new Notification and use accessors to set the note name
    struct INotification *notification = puremvc_notification_new("TestNote", NULL, NULL);

    // test assertions
    assert(strcmp(notification->getName(notification), "TestNote") == 0);

    puremvc_notification_free(&notification);
    assert(notification == NULL);
}

/**
 * Tests setting and getting the body using Notification class accessor methods.
 */
void testBodyAccessors() {
    // Create a new Notification and use accessors to set the body
    struct { int value; } *vo = malloc(sizeof(*vo));
    if (vo) vo->value = 5;

    // Create a new Notification and use accessors to set the body
    struct INotification *notification = puremvc_notification_new("TestNote", NULL, NULL);
    notification->setBody(notification, vo);

    // test assertions
    assert(notification->getBody(notification) == vo);
    puremvc_notification_free(&notification);
    assert(notification == NULL);
}

/**
 * Tests setting and getting the type using Notification class accessor methods.
 */
void testTypeAccessors() {
    struct INotification *notification = puremvc_notification_new("TestNote", NULL, "TestNoteType");

    // test assertions
    assert(strcmp(notification->getType(notification), "TestNoteType") == 0);
    notification->setType(notification, "TestNoteType2");
    assert(strcmp(notification->getType(notification), "TestNoteType2") == 0);

    puremvc_notification_free(&notification);
    assert(notification == NULL);
}

/** Node Notifications */
void testToString() {
    struct { int value; } *test = malloc(sizeof(*test));
    if (test) test->value = 5;

    struct INotification *notification = puremvc_notification_new("TestNote", test, "TestNoteType");

    const char *str = notification->toString(notification);
    assert(strcmp(str, "TestNoteTestNoteType") == 0);

    puremvc_notification_free(&notification);
    assert(notification == NULL);
}

int main() {
    testConstructor();
    testNameAccessors();
    testBodyAccessors();
    testTypeAccessors();
    testToString();
    return 0;
}
