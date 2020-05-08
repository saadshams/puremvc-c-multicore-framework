#include "NotificationTest.h"
#include "interfaces/Notification.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

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
    Notification *notification = NewNotification(NULL, NULL, NULL);
    notification->setBody(notification, (void*)5);
    assert((int)notification->getBody(notification) == 5);
    DeleteNotification(notification);
}

void testConstructor() {
    typedef struct {} Temp;
    Temp *temp = malloc(sizeof(Temp));
    Notification *notification = NewNotification("TestNote", temp, "TestNoteType");
    assert(strcmp(notification->getName(notification), "TestNote") == 0);
    assert(notification->getBody(notification) == temp);
    assert(strcmp(notification->getType(notification), "TestNoteType") == 0);
    DeleteNotification(notification);
}