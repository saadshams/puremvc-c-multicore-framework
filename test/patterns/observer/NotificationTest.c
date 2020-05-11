#include "NotificationTest.h"
#include "interfaces/Notification.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>

int main() {
    testConstructor();
    testNameAccessors();
    testBodyAccessors();
    puts("NotificationTest: Success");
    return 0;
}

void testNameAccessors() {
    Notification *notification = NewNotification("TestNote", NULL, NULL);
    assert(strcmp(notification->getName(notification), "TestNote") == 0);
    DeleteNotification(notification);
}

void testBodyAccessors() {
    struct {
        int value;
    } test = {5};
    Notification *notification = NewNotification("TestNote", NULL, NULL);
    notification->setBody(notification, &test);
    assert(notification->getBody(notification) == &test);
    DeleteNotification(notification);
}

void testConstructor() {
    typedef struct {int value;} Test;
    Test test = {5};
    Notification *notification = NewNotification("TestNote", &test, "TestNoteType");
    assert(strcmp(notification->getName(notification), "TestNote") == 0);
    assert(notification->getBody(notification) == &test);
    assert(strcmp(notification->getType(notification), "TestNoteType") == 0);
    DeleteNotification(notification);
}