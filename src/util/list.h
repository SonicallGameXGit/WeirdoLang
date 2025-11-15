#ifndef __UTIL__LIST_H__
#define __UTIL__LIST_H__

#include <stdio.h>
#include <stdlib.h>

typedef struct List {
    void **items;
    size_t length;
    size_t capacity;
} List;
void *List_push(List *list, void *item) {
    if (list->length >= list->capacity) {
        size_t capacity = (list->capacity == 0) ? 4 : list->capacity * 2;
        list->items = (void **)realloc(list->items, capacity * sizeof(void *));
        if (list->items == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
        list->capacity = capacity;
    }
    
    return list->items[list->length++] = item;
}
void *List_get(const List *list, size_t index) {
    if (index >= list->length) {
        fprintf(stderr, "Index out of bounds: %zu >= %zu\n", index, list->length);
        exit(EXIT_FAILURE);
    }
    return list->items[index];
}
void List_free(List *list) {
    free(list->items);
    list->items = NULL;
    list->length = 0;
    list->capacity = 0;
}

#endif