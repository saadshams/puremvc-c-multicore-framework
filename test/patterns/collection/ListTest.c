#include "ListTest.h"
#include "interfaces/List.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    testAddFirst();
    testAddLast();
    testRemove();
    testContainsValue();
    testClone();
    testIsEmpty();
    testSize();
    testClear();
    testPrint();
    testPop();
    testPush();
    testRemoveFirst();
    testRemoveLast();
    testShift();
    testUnshift();
    puts("ListTest: Success");
    return 0;
}

typedef struct Test Test;

struct Test {
    char *name;
    int data;
    Test *next;
};

void testAddFirst() {
    static List *list;

    Test *test1 = &(Test) {"name1", 1};
    Test *test2 = &(Test) {"name2", 2};
    Test *test3 = &(Test) {"name3", 3};

    assert($List.addFirst(&list, test1) == test1);
    assert($List.addFirst(&list, test2) == test2);
    assert($List.addFirst(&list, test3) == test3);

    assert(list->data == test3);
    assert(list->next->data == test2);
    assert(list->next->next->data == test1);
}

void testAddLast() {
    static List *list;

    Test *test1 = &(Test) {"name1", 1};
    Test *test2 = &(Test) {"name2", 2};
    Test *test3 = &(Test) {"name3", 3};

    assert($List.addLast(&list, test1) == test1);
    assert($List.addLast(&list, test2) == test2);
    assert($List.addLast(&list, test3) == test3);
}

void testRemove() {
    static List *list;

    Test *test1 = &(Test) {"name1", 1};
    Test *test2 = &(Test) {"name2", 2};
    Test *test3 = &(Test) {"name3", 3};

    $List.addLast(&list, test1);
    $List.addLast(&list, test2);
    $List.addLast(&list, test3);

    assert($List.remove(&list, test2) == test2);
    assert($List.remove(&list, test1) == test1);
    assert($List.remove(&list, test3) == test3);
    assert($List.remove(&list, &(Test) {"name4", 4}) == NULL);
}

void testContainsValue() {
    static List *list;

    assert($List.containsValue(&list, NULL) == false);
    assert($List.containsValue(&list, "emptyList") == false);

    Test *test1 = &(Test) {"name1", 1};
    Test *test2 = &(Test) {"name2", 2};
    Test *test3 = &(Test) {"name3", 3};

    $List.addLast(&list, test1);
    $List.addLast(&list, test2);
    $List.addLast(&list, test3);

    assert($List.containsValue(&list, test1) == true);
    assert($List.containsValue(&list, test2) == true);
    assert($List.containsValue(&list, test3) == true);
    assert($List.containsValue(&list, &(Test) {"name4", 4}) == false);

    $List.remove(&list, test2);
    assert($List.containsValue(&list, test2) == false);

    $List.remove(&list, test3);
    assert($List.containsValue(&list, test3) == false);

    $List.remove(&list, test1);
    assert($List.containsValue(&list, test1) == false);
}

void testClone() {
    static List *list;

    Test *test1 = &(Test) {"name1", 1};
    Test *test2 = &(Test) {"name2", 2};
    Test *test3 = &(Test) {"name3", 3};

    $List.addLast(&list, test1);
    $List.addLast(&list, test2);
    $List.addLast(&list, test3);

    assert($List.size(&list) == 3);

    List *clone = $List.clone(&list);

    assert($List.size(&clone) == 3);
}

void testIsEmpty() {
    static List *list;

    assert($List.isEmpty(&list));

    Test *test1 = &(Test) {"name1", 1};
    Test *test2 = &(Test) {"name2", 2};
    Test *test3 = &(Test) {"name3", 3};

    assert($List.addFirst(&list, test1) == test1);
    assert($List.addFirst(&list, test2) == test2);
    assert($List.addFirst(&list, test3) == test3);

    assert(!$List.isEmpty(&list));
}

void testSize() {
    static List *list;

    Test *test1 = &(Test) {"name1", 1};
    Test *test2 = &(Test) {"name2", 2};
    Test *test3 = &(Test) {"name3", 3};

    $List.addLast(&list, test1);
    assert($List.size(&list) == 1);

    $List.addLast(&list, test2);
    assert($List.size(&list) == 2);

    $List.addLast(&list, test3);
    assert($List.size(&list) == 3);

    $List.remove(&list, test2);
    assert($List.size(&list) == 2);

    $List.remove(&list, test1);
    $List.remove(&list, test2);
    assert($List.size(&list) == 1);

    $List.remove(&list, test3);
    assert($List.size(&list) == 0);
}

void testClear() {
    static List *list;

    Test *test1 = malloc(sizeof(List));
    Test *test2 = malloc(sizeof(List));
    Test *test3 = malloc(sizeof(List));

    test1->next = NULL;
    test2->next = NULL;
    test3->next = NULL;

    $List.addLast(&list, test1);
    $List.addLast(&list, test2);
    $List.addLast(&list, test3);

    $List.clear(&list);
    assert($List.size(&list) == 0);
    assert(list == NULL);
}

void testPrint() {
    static List *list;

    Test *test1 = &(Test) {"name1", 1};
    Test *test2 = &(Test) {"name2", 2};
    Test *test3 = &(Test) {"name3", 3};

    $List.addLast(&list, test1);
    $List.addLast(&list, test2);
    $List.addLast(&list, test3);

    $List.print(&list);
}

void testPop() {
    static List *list;

    Test *test1 = &(Test) {"name1", 1};
    Test *test2 = &(Test) {"name2", 2};
    Test *test3 = &(Test) {"name3", 3};

    $List.addLast(&list, test1);
    $List.addLast(&list, test2);
    $List.addLast(&list, test3);

    assert($List.pop(&list) == test3);
    assert($List.pop(&list) == test2);
    assert($List.pop(&list) == test1);
    assert($List.pop(&list) == NULL);
}

void testPush() {
    static List *list;

    Test *test1 = &(Test) {"name1", 1};
    Test *test2 = &(Test) {"name2", 2};
    Test *test3 = &(Test) {"name3", 3};

    $List.push(&list, test1);
    $List.push(&list, test2);
    $List.push(&list, test3);

    assert($List.pop(&list) == test3);
    assert($List.pop(&list) == test2);
    assert($List.pop(&list) == test1);
    assert($List.pop(&list) == NULL);
}

void testRemoveFirst() {
    static List *list;

    Test *test1 = &(Test) {"name1", 1};
    Test *test2 = &(Test) {"name2", 2};
    Test *test3 = &(Test) {"name3", 3};

    $List.addLast(&list, test1);
    $List.addLast(&list, test2);
    $List.addLast(&list, test3);

    assert($List.removeFirst(&list) == test1);
    assert($List.removeFirst(&list) == test2);
    assert($List.removeFirst(&list) == test3);
    assert($List.removeFirst(&list) == NULL);
}

void testRemoveLast() {
    static List *list;

    Test *test1 = &(Test) {"name1", 1};
    Test *test2 = &(Test) {"name2", 2};
    Test *test3 = &(Test) {"name3", 3};

    $List.addLast(&list, test1);
    $List.addLast(&list, test2);
    $List.addLast(&list, test3);

    assert($List.removeLast(&list) == test3);
    assert($List.removeLast(&list) == test2);
    assert($List.removeLast(&list) == test1);
    assert($List.removeLast(&list) == NULL);
}

void testShift() {
    static List *list;

    Test *test1 = &(Test) {"name1", 1};
    Test *test2 = &(Test) {"name2", 2};
    Test *test3 = &(Test) {"name3", 3};

    $List.addLast(&list, test1);
    $List.addLast(&list, test2);
    $List.addLast(&list, test3);

    assert($List.shift(&list) == test1);
    assert($List.shift(&list) == test2);
    assert($List.shift(&list) == test3);
    assert($List.shift(&list) == NULL);
}

void testUnshift() {
    static List *list;

    Test *test1 = &(Test) {"name1", 1};
    Test *test2 = &(Test) {"name2", 2};
    Test *test3 = &(Test) {"name3", 3};

    assert($List.unshift(&list, test1) == test1);
    assert($List.unshift(&list, test2) == test2);
    assert($List.unshift(&list, test3) == test3);

    assert(list->data == test3);
    assert(list->next->data == test2);
    assert(list->next->next->data == test1);
    assert(list->next->next->next == NULL);
}
