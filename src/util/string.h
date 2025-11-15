#ifndef __UTIL__STRING_H__
#define __UTIL__STRING_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct {
    char *data;
    size_t length;
    size_t capacity;
} String;
void String_from(String *str, const char *cstr) {
    str->data = (char *)malloc(strlen(cstr) + 1);
    if (str->data == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    strcpy(str->data, cstr);
    str->length = strlen(cstr);
    str->capacity = str->length + 1;
}
void String_fromN(String *str, const char *cstr, size_t n) {
    str->data = (char *)malloc(n + 1);
    if (str->data == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    strncpy(str->data, cstr, n);
    str->data[n] = '\0';
    str->length = n;
    str->capacity = n + 1;
}
void String_copy(String *dest, const String *src) {
    dest->data = (char *)malloc(src->length + 1);
    if (dest->data == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    strcpy(dest->data, src->data);
    dest->length = src->length;
    dest->capacity = src->length + 1;
}
void String_append(String *str, const char *cstr) {
    size_t cstrLength = strlen(cstr);
    if (str->length + cstrLength >= str->capacity) {
        str->capacity = str->length + cstrLength + 1;
        str->data = (char *)realloc(str->data, str->capacity);
        if (str->data == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
    }
    strcpy(str->data + str->length, cstr);
    str->length += cstrLength;
}
void String_free(String *str) {
    free(str->data);
    str->data = NULL;
    str->length = 0;
    str->capacity = 0;
}

#endif