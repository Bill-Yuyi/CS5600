//
// Created by Yuyi Wang on 4/2/24.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "table.h"
#include "polybius.h"

char* read_file(const char* filename, long* size) {
    char* buffer;
    FILE* file = fopen(filename, "r");
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

    table_t *table = (table_t *)malloc(sizeof(table_t));
    if (table == NULL)
    {
        free(buffer);
        fclose(file);
        return NULL;
    }
    initializeTable(table);

    char* res = pbDecode(buffer, table);

    return res;
}

int write_file(const char* file_path, const char* content, size_t size) {
    FILE* file = fopen(file_path, "w");
    if(!file) {
        perror("Error opening file for writing");
        return 1;
    }

    table_t *table = (table_t *)malloc(sizeof(table_t));
    if (table == NULL)
    {
        return -1;
    }
    initializeTable(table);

    char* encoded_text = pbEncode(content, table);
    printf("%s, %s", content, encoded_text);
    size_t encoded_size = strlen(encoded_text);
    size_t written = fwrite(encoded_text, 1, encoded_size, file);
    if(written != encoded_size) {
        perror("Error writing content");
        fclose(file);
        return -1;
    }
    fclose(file);
    free(encoded_text);
    free(table);
    return 0;
}

void set_permission(const char* remote_path, const char* permission) {
    char meta_path[1024];
    snprintf(meta_path, sizeof(meta_path), "%s.meta", remote_path);
    FILE* meta_file = fopen(meta_path, "w");
    fprintf(meta_file, "%s\n", permission);
    fclose(meta_file);
}

