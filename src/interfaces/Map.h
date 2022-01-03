#ifndef PUREMVC_MAP_H
#define PUREMVC_MAP_H

#include <stdbool.h>

typedef struct Map Map;

struct Map {
    const char *key;
    const void *value;
    Map *next;
};

struct $Map {
    const void *(*get)(Map **head, const char *key);

    bool (*containsKey)(Map **head, const char *key);

    bool (*containsValue)(Map **head, const void *value);

    const void *(*put)(Map **head, const char *key, const void *value);

    const void *(*replace)(Map **head, const char *key, const void *value);

    const void *(*remove)(Map **head, const char *key);

    int (*size)(Map **head);
};

const struct $Map $Map;

#endif //PUREMVC_MAP_H
