#include "NotificationTest.h"
#include "interfaces/Notification.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>

/**
 * Test the PureMVC Notification class.
 *
 * @see Notification
 */
int main() {
    testConstructor();
    testNameAccessors();
    testBodyAccessors();
    testToString();
    puts("NotificationTest: Success");
    return 0;
}

/**
 * Tests setting and getting the name using Notification class accessor methods.
 */
void testNameAccessors() {
    // Create a new Notification and use accessors to set the note name
    Notification *notification = NewNotification("TestNote", NULL, NULL);

    // test assertions
    assert(strcmp(notification->getName(notification), "TestNote") == 0);
    DeleteNotification(notification);
}

/**
 * Tests setting and getting the body using Notification class accessor methods.
 */
void testBodyAccessors() {
    // Create a new Notification and use accessors to set the body
    struct {int value;} test = {5};

    // Create a new Notification and use accessors to set the body
    Notification *notification = NewNotification("TestNote", NULL, NULL);
    notification->setBody(notification, &test);

    // test assertions
    assert(notification->getBody(notification) == &test);
    DeleteNotification(notification);
}

/**
 * Tests setting the name and body using the Notification class Constructor.
 */
void testConstructor() {
    // Create a new Notification using the Constructor to set the note name and body
    struct {int value;} test = {5};
    Notification *notification = NewNotification("TestNote", &test, "TestNoteType");

    // test assertions
    assert(strcmp(notification->getName(notification), "TestNote") == 0);
    assert(notification->getBody(notification) == &test);
    assert(strcmp(notification->getType(notification), "TestNoteType") == 0);
    DeleteNotification(notification);
}

/** Test Notifications */
void testToString() {
    struct {int value;} test = {5};
    Notification *notification = NewNotification("TestNote", &test, "TestNoteType");
    const char *str = notification->toString(notification);
    assert(strcmp(str, "TestNoteTestNoteType") == 0);
}