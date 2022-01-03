#include "interfaces/List.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

// mutex for modelMap
static pthread_rwlock_t mutex;

// Inserts the specified element at the beginning of this list (unshift).
static const void *$addFirst(List **head, const void *data) {
    List *node = malloc(sizeof(List));
    if (node == NULL) goto exception;

    node->data = data;
    node->next = *head;
    *head = node;
    return data;

    exception:
    pthread_rwlock_unlock(&mutex);
    fprintf(stderr, "List allocation failed.\n");
    return NULL;
}

// Inserts the specified element at the beginning of this list
static const void *$unshift(List **head, const void *data) {
    return $addFirst(head, data);
}

// Appends the specified element to the end of this list (push).
static const void *$addLast(List **head, const void *data) {
    pthread_rwlock_wrlock(&mutex);
    List **cursor = head;
    while (*cursor)
        cursor = &(*cursor)->next;

    List *node = malloc(sizeof(List));
    if (node == NULL) goto exception;

    node->data = data;
    node->next = NULL;
    *cursor = node;
    pthread_rwlock_unlock(&mutex);
    return node->data;

    exception:
    pthread_rwlock_unlock(&mutex);
    fprintf(stderr, "List allocation failed.\n");
    return NULL;
}

static bool $containsValue(List **head, const void *data) {
    pthread_rwlock_rdlock(&mutex);
    List *cursor = *head;
    while (cursor) {
        if (cursor->data == data) {
            pthread_rwlock_unlock(&mutex);
            return true;
        }
        cursor = cursor->next;
    }
    pthread_rwlock_unlock(&mutex);
    return false;
}

static List *$clone(List **head) {
    pthread_rwlock_wrlock(&mutex);
    List *cursor = *head;

    List *copy = NULL;
    List **tail = &copy;

    while (cursor) {
        List *list = malloc(sizeof(List));
        if (list == NULL) goto exception;

        list->data = cursor->data;
        list->next = NULL;

        *tail = list;

        tail = &(*tail)->next;
        cursor = cursor->next;
    }
    pthread_rwlock_unlock(&mutex);
    return copy;

    exception:
    pthread_rwlock_unlock(&mutex);
    fprintf(stderr, "List allocation failed.\n");
    return NULL;
}

static void $clear(List **head) {
    pthread_rwlock_wrlock(&mutex);
    List **cursor = head;
    while (*cursor) {
        List *node = *cursor;
        cursor = &(*cursor)->next;
        free(node);
    }
    *head = NULL;
    pthread_rwlock_unlock(&mutex);
}

static bool $isEmpty(List **head) {
    return *head == NULL;
}

static int $size(List **head) {
    pthread_rwlock_rdlock(&mutex);
    int n = 0;
    List *cursor = *head;
    while (cursor && ++n)
        cursor = cursor->next;
    pthread_rwlock_unlock(&mutex);
    return n;
}

static void $print(List **head) {
    pthread_rwlock_rdlock(&mutex);
    List *cursor = *head;
    while (cursor) {
        printf("%p -> ", cursor->data);
        cursor = cursor->next;
    }
    puts("null");
    pthread_rwlock_unlock(&mutex);
}

// Pushes an element (add to the end of an array) onto the stack represented by this list.
static const void *$push(List **head, const void *data) {
    return $addLast(head, data);
}

static const void *$remove(List **head, const void *data) {
    pthread_rwlock_wrlock(&mutex);
    List **cursor = head;
    while (*cursor) {
        if ((*cursor)->data == data) {
            List *list = *cursor;
            *cursor = (*cursor)->next;
            pthread_rwlock_unlock(&mutex);
            return list->data;
        }
        cursor = &(*cursor)->next;
    }
    pthread_rwlock_unlock(&mutex);
    return NULL;
}

// Removes and returns the first element from this list (shift).
static const void *$removeFirst(List **head) {
    List *node = *head;
    if (node) {
        *head = (*head)->next;
        return node->data;
    }
    return NULL;
}

// Removes and returns the first element from this list.
static const void *$shift(List **head) {
    return $removeFirst(head);
}

// Removes and returns the last element from this list.
static const void *$removeLast(List **head) {
    List **cursor = head;
    while (*cursor) {
        if ((*cursor)->next == NULL) {
            List *node = *cursor;
            *cursor = NULL;
            return node->data;
        }
        cursor = &(*cursor)->next;
    }
    return NULL;
}

// Pops an element (remove from the end of an array) from the stack represented by this list.
static const void *$pop(List **head) {
    return $removeLast(head);
}

const struct $List $List = {.addFirst = $addFirst, .addLast = $addLast,
        .clone = $clone, .clear = $clear, .isEmpty = $isEmpty, .print = $print,
        .pop = $pop, .push = $push, .shift = $shift, .unshift = $unshift,
        .containsValue = $containsValue, .size = $size,
        .remove = $remove, .removeFirst = $removeFirst, .removeLast = $removeLast};

