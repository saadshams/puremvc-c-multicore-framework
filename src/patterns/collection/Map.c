#include "interfaces/Map.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// mutex for modelMap
static pthread_rwlock_t mutex;

// Returns true if this collection contains a mapping for the specified key.
static bool $containsKey(Map **head, const char *key) {
    pthread_rwlock_rdlock(&mutex);
    Map *cursor = *head;
    while (cursor && strcmp(cursor->key, key) != 0)
        cursor = cursor->next;
    pthread_rwlock_unlock(&mutex);
    return cursor != NULL;
}

// Returns true if this maps a key to the specified value.
bool containsValue(Map **head, const void *value) {
    pthread_rwlock_rdlock(&mutex);
    Map *cursor = *head;
    while (cursor && cursor->value != value)
        cursor = cursor->next;
    pthread_rwlock_unlock(&mutex);
    return cursor != NULL;
}

// Returns the value to which the specified key is mapped, or null if this collection contains no mapping for the key.
static const void *$get(Map **head, const char *key) {
    Map *cursor = *head;
    while (cursor && strcmp(cursor->key, key) != 0)
        cursor = cursor->next;
    return cursor != NULL ? cursor->value : NULL;
}

// Associates the specified value with the specified key in this collection
static const void *$put(Map **head, const char *key, const void *value) {
    if ($containsKey(head, key)) return NULL;

    pthread_rwlock_wrlock(&mutex);
    Map **cursor = head;
    while (*cursor)
        cursor = &(*cursor)->next;

    Map *node = malloc(sizeof(Map));
    if (node == NULL) goto exception;

    node->key = key;
    node->value = value;
    node->next = NULL;
    *cursor = node;
    pthread_rwlock_unlock(&mutex);
    return node->value;

    exception:
    pthread_rwlock_unlock(&mutex);
    fprintf(stderr, "Map allocation failed.\n");
    return NULL;
}

// Replaces the entry for the specified key only if it is currently mapped to some value.
static const void *$replace(Map **head, const char *key, const void *value) {
    pthread_rwlock_wrlock(&mutex);
    Map *cursor = *head;
    while (cursor) {
        if (strcmp(cursor->key, key) == 0) {
            const void *v = cursor->value;
            cursor->value = value;
            pthread_rwlock_unlock(&mutex);
            return v;
        }
        cursor = cursor->next;
    }
    pthread_rwlock_unlock(&mutex);
    return NULL;
}

// Removes the mapping for a key from this collection if it is present (optional operation).
static const void *$remove(Map **head, const char *key) {
    pthread_rwlock_wrlock(&mutex);
    Map **cursor = head;
    while (*cursor) {
        if (strcmp((*cursor)->key, key) == 0) {
            Map *node = *cursor;
            *cursor = (*cursor)->next;
            pthread_rwlock_unlock(&mutex);
            return node->value;
        }
        cursor = &(*cursor)->next;
    }
    pthread_rwlock_unlock(&mutex);
    return NULL;
}

// Returns the number of key-value mappings in this collection.
static int size(Map **head) {
    pthread_rwlock_rdlock(&mutex);
    int n = 0;
    Map *cursor = *head;
    while (cursor && ++n)
        cursor = cursor->next;
    pthread_rwlock_unlock(&mutex);
    return n;
}

const struct $Map $Map = {.get = $get, .put = $put,
        .containsKey = $containsKey, .containsValue = containsValue,
        .replace = $replace, .remove = $remove,
        .size = size};
