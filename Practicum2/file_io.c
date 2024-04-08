//
// Created by Yuyi Wang on 4/2/24.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "table.h"
#include "polybius.h"

/**
 * Reads the content of a file, decodes it using the Polybius table, and returns the decoded content.
 * Allocates memory for the decoded content that needs to be freed by the caller.
 *
 * @param filename Path to the file to be read.
 * @param size Pointer to store the size of the read content.
 * @return Pointer to the decoded content on success, NULL on error.
 */
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

/**
 * Writes the given content to a file after encoding it using the Polybius table.
 *
 * @param file_path Path where the content is to be written.
 * @param content The content to be encoded and written.
 * @param size The size of the content to be written (unused, consider removing if not needed).
 * @return 0 on success, non-zero value on error.
 */
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

/**
 * Writes the given content to a file without encoding.
 * This function can be used for information that doesn't require encoding.
 *
 * @param file_path Path where the content is to be written.
 * @param content The content to be written.
 * @param size The size of the content to be written (unused, consider removing if not needed).
 * @return 0 on success, non-zero value on error.
 */
int write_file_without_encode(const char* file_path, const char* content, size_t size) {
    FILE* file = fopen(file_path, "w");
    if(!file) {
        perror("Error opening file for writing");
        return 1;
    }


    size_t written = fwrite(content, 1, strlen(content), file);
    if(written != strlen(content)) {
        perror("Error writing content");
        fclose(file);
        return -1;
    }
    fclose(file);

    return 0;
}

/**
 * Sets the permission for a given file by writing the permission value to a metadata file associated with the file.
 *
 * @param remote_path Path to the file whose permission is to be set.
 * @param permission The permission value to be set ('0' for read-only, '1' for read-write).
 */
void set_permission(const char* remote_path, const char* permission) {
    char meta_path[1024];
    snprintf(meta_path, sizeof(meta_path), "%s.meta", remote_path);
    FILE* meta_file = fopen(meta_path, "w");
    fprintf(meta_file, "%s\n", permission);
    fclose(meta_file);
}

