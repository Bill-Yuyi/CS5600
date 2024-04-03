//
// Created by Yuyi Wang on 4/2/24.
//

#include <stdio.h>
#include <stdlib.h>

char* read_file(const char* filename, long* size) {
    char* buffer;
    FILE* file = fopen(filename, "rb");
    if(!file) {
        perror("Error when opening the file");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    *size = ftell(file);
    rewind(file);

    buffer = (char*) malloc(*size + 1);
    if(buffer == NULL) {
        perror("Error when allocating memory for reading buffer");
        fclose(file);
        return NULL;
    }

    if(fread(buffer, *size, 1, file) != 1) {
        free(buffer);
        fclose(file);
        return NULL;
    }

    fclose(file);
    buffer[*size] ='\0';
    return buffer;
}

int write_file(const char* file_path, const char* content, size_t size) {
    FILE* file = fopen(file_path, "wb");
    if(!file) {
        perror("Error opening file for writing");
        return 1;
    }

    size_t written = fwrite(content, 1, size, file);
    if(written != size) {
        perror("Error writing content");
        fclose(file);
        return -1;
    }

    fclose(file);
    return 0;
}