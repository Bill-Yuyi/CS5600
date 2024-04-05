//
// Created by Yuyi Wang on 4/3/24.
//

#include "file_io.h"
#include <string.h>
#include <stdlib.h>
#include "global_mutex.h"

void write_request(char* rest, char* remote_path,char server_message[]) {
    pthread_mutex_lock(&file_operation_mutex);

    long file_size = atol(strtok_r(rest, "|", &rest));
    char* file_content = rest;

    if(write_file(remote_path, file_content, file_size) == 0) {
        printf("Content written to %s successfully\n", remote_path);
        strcpy(server_message, "Writing file success!");
    }else {
        printf("Failed to write content to %s\n", remote_path);
        strcpy(server_message, "Failed to write content to %s\n", remote_path);
    }

    pthread_mutex_unlock(&file_operation_mutex);
}

void get_request(char* remote_path, long file_size,char server_message[]) {
    pthread_mutex_lock(&file_operation_mutex);

    char* content = read_file(remote_path, &file_size);
    if(content == NULL) {
        strcpy(server_message, "No such file");
    }else {
        strcpy(server_message, content);
        free(content);
    }
    pthread_mutex_unlock(&file_operation_mutex);
}

void remove_request(char* remote_path, char server_message[]) {
    pthread_mutex_lock(&file_operation_mutex);
    if (remove(remote_path) == 0) {
        strcpy(server_message, "File deleted!");
    } else {
        strcpy(server_message, "Error deleting file!");
    }
    pthread_mutex_unlock(&file_operation_mutex);
}