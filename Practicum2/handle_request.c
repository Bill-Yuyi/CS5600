//
// Created by Yuyi Wang on 4/3/24.
//

#include "file_io.h"
#include <string.h>
#include <stdlib.h>

void write_request(char* rest, char* remote_path,char server_message[]) {
    long file_size = atol(strtok_r(rest, "|", &rest));
    char* file_content = rest;
    if(write_file(remote_path, file_content, file_size) == 0) {
        printf("Content written to %s successfully\n", remote_path);
    }else {
        printf("Failed to write content to %s\n", remote_path);
    }
    strcpy(server_message, "Writing file success!");
}

void get_request(char* remote_path, long file_size,char server_message[]) {
    char* content = read_file(remote_path, &file_size);
    if(content == NULL) {
        perror("memory allocation for file content is failed");
        exit(1);
    }
    strcpy(server_message, content);
    free(content);
}