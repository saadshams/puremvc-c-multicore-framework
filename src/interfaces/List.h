#ifndef PUREMVC_LIST_H
#define PUREMVC_LIST_H

#include <stdbool.h>

typedef struct List List;

struct List {
    const void *data;
    List *next;
};

struct $List {
    const void *(*addFirst)(List **head, const void *data);

    const void *(*addLast)(List **head, const void *data);

    bool (*containsValue)(List **head, const void *data);

    List *(*clone)(List **head);

    void (*clear)(List **head);

    bool (*isEmpty)(List **head);

    const void *(*pop)(List **head);

    const void *(*push)(List **head, const void *data);

    void (*print)(List **head);

    const void *(*remove)(List **head, const void *data);

    const void *(*removeFirst)(List **head);

    const void *(*removeLast)(List **head);

    int (*size)(List **head);

    const void *(*shift)(List **head);

    const void *(*unshift)(List **head, const void *data);
};

const struct $List $List;

#endif //PUREMVC_LIST_H
