#include "MapTest.h"
#include "interfaces/Map.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

int main() {
    testPut();
    testGet();
    testContainsKey();
    testContainsValue();
    testReplace();
    testRemove();
    testSize();
    puts("MapTest: Success");
    return 0;
}

struct Test {
    char *name;
    int data;
};

typedef struct Test Test;

void testPut() {
    static Map *map;

    Test *test1 = &(Test) {"name1", 1};
    Test *test2 = &(Test) {"name2", 2};
    Test *test3 = &(Test) {"name3", 3};

    assert($Map.put(&map, test1->name, test1) == test1);
    assert($Map.put(&map, test2->name, test2) == test2);
    assert($Map.put(&map, test3->name, test3) == test3);
}

void testGet() {
    static Map *map;

    Test *test1 = &(Test) {"name1", 1};
    Test *test2 = &(Test) {"name2", 2};
    Test *test3 = &(Test) {"name3", 3};

    $Map.put(&map, test1->name, test1);
    $Map.put(&map, test2->name, test2);
    $Map.put(&map, test3->name, test3);

    assert($Map.get(&map, test1->name) == test1);
    assert($Map.get(&map, test2->name) == test2);
    assert($Map.get(&map, test3->name) == test3);
    assert($Map.get(&map, "unknown") == NULL);
}

void testContainsKey() {
    static Map *map;

    Test *test1 = &(Test) {"name1", 1};
    Test *test2 = &(Test) {"name2", 2};
    Test *test3 = &(Test) {"name3", 3};

    $Map.put(&map, test1->name, test1);
    $Map.put(&map, test2->name, test2);
    $Map.put(&map, test3->name, test3);

    assert($Map.containsKey(&map, test1->name) == true);
    assert($Map.containsKey(&map, test2->name) == true);
    assert($Map.containsKey(&map, test2->name) == true);
    assert($Map.containsKey(&map, "invalid") == false);
}

void testContainsValue() {
    static Map *map;

    Test *test1 = &(Test) {"name1", 1};
    Test *test2 = &(Test) {"name2", 2};
    Test *test3 = &(Test) {"name3", 3};

    $Map.put(&map, test1->name, test1);
    $Map.put(&map, test2->name, test2);
    $Map.put(&map, test3->name, test3);

    assert($Map.containsValue(&map, test3));
    assert($Map.containsValue(&map, test2));
    assert($Map.containsValue(&map, test1));

    Test *test4 = &(Test) {"name4", 4};
    assert($Map.containsValue(&map, test4) == false);

    Test test5 = {"name5", 5};
    assert($Map.containsValue(&map, &test5) == false);
}

void testReplace() {
    static Map *map;

    Test *test1 = &(Test) {"name1", 1};
    Test *test2 = &(Test) {"name2", 2};
    Test *test3 = &(Test) {"name3", 3};

    $Map.put(&map, test1->name, test1);
    $Map.put(&map, test2->name, test2);
    $Map.put(&map, test3->name, test3);

    Test *test4 = &(Test) {"name4", 4};

    Test *oldValue = (Test *) ($Map.replace(&map, "name3", test4));

    assert(oldValue == test3);
    assert(strcmp(oldValue->name, "name3") == 0);
    assert($Map.containsKey(&map, "name3") == true);
    assert($Map.get(&map, "name3") == test4);
    assert(&test3 != NULL);
}

void testRemove() {
    static Map *map;

    Test *test1 = &(Test) {"name1", 1};
    Test *test2 = &(Test) {"name2", 2};
    Test *test3 = &(Test) {"name3", 3};

    $Map.put(&map, test1->name, test1);
    $Map.put(&map, test2->name, test2);
    $Map.put(&map, test3->name, test3);

    assert($Map.remove(&map, test2->name) == test2);
    assert($Map.remove(&map, test3->name) == test3);
    assert($Map.remove(&map, test1->name) == test1);
}

void testSize() {
    static Map *map;

    Test *test1 = &(Test) {"name1", 1};
    Test *test2 = &(Test) {"name2", 2};
    Test *test3 = &(Test) {"name3", 3};

    $Map.put(&map, "name1", test1);
    $Map.put(&map, "name1", test2);
    $Map.put(&map, "name1", test3);

    assert($Map.size(&map) == 1);

    $Map.put(&map, "name2", test2);
    $Map.put(&map, "name2", test3);

    assert($Map.size(&map) == 2);

    $Map.put(&map, "name3", test3);

    assert($Map.size(&map) == 3);
}
