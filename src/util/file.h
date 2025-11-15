#ifndef __UTIL__FILE_H__
#define __UTIL__FILE_H__

#include <util/string.h>

void File_read(const char *path, String *output) {
    FILE *file = fopen(path, "rb");
    if (file == NULL) {
        fprintf(stderr, "Failed to open file: %s\n", path);
        exit(EXIT_FAILURE);
    }
    fseek(file, 0, SEEK_END);
    size_t fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    output->data = (char *)malloc(fileSize + 1);
    if (output->data == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    fread(output->data, 1, fileSize, file);
    output->data[fileSize] = '\0';
    output->length = fileSize;
    output->capacity = fileSize + 1;

    fclose(file);
}
void File_write(const char *path, const String *input) {
    FILE *file = fopen(path, "wb");
    if (file == NULL) {
        fprintf(stderr, "Failed to open file for writing: %s\n", path);
        exit(EXIT_FAILURE);
    }

    fwrite(input->data, 1, input->length, file);
    fclose(file);
}

#endif