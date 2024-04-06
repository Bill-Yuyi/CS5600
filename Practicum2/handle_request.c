//
// Created by Yuyi Wang on 4/3/24.
//

#include "file_io.h"
#include <string.h>
#include <stdlib.h>
#include "global_mutex.h"

int is_writable(const char* filename) {
    char meta_path[1024];
    snprintf(meta_path, sizeof(meta_path), "%s.meta", filename);
    FILE* meta_file = fopen(meta_path, "r");
    if(!meta_file) return 2; // first time creation

    char buffer[100];
    fgets(buffer, sizeof(buffer), meta_file);
    fclose(meta_file);

    if(strstr(buffer, "1")) {
        return 1;
    }else {
        return 0;
    }
}

void write_request(char* rest, char* remote_path,char server_message[]) {
    pthread_mutex_lock(&file_operation_mutex);

    int permission = is_writable(remote_path);
    if(!permission) {
        strcpy(server_message, "the file is not writable");
        pthread_mutex_unlock(&file_operation_mutex);
        return;
    }

    long file_size = atol(strtok_r(rest, "|", &rest));
    char* file_content = rest;

    if(write_file(remote_path, file_content, file_size) == 0) {

        printf("Content written to %s successfully\n", remote_path);
        strcpy(server_message, "Writing file success!");
    }else {
        printf("Failed to write content to %s\n", remote_path);
        strcpy(server_message, "Failed to write content\n");
    }

    pthread_mutex_unlock(&file_operation_mutex);
}

void write_request_with_permission(char* rest, char* remote_path,char server_message[]) {
    pthread_mutex_lock(&file_operation_mutex);

    long file_size = atol(strtok_r(rest, "|", &rest));
    char* file_content = strtok_r(rest, "|", &rest);
    char* permission_set = rest;
    if(permission_set == NULL) {
        permission_set = "1";
    }
    if(write_file(remote_path, file_content, file_size) == 0) {
        set_permission(remote_path, permission_set);
        printf("Content written to %s successfully\n", remote_path);
        strcpy(server_message, "Writing file success!");
    }else {
        printf("Failed to write content to %s\n", remote_path);
        strcpy(server_message, "Failed to write content to %s\n");
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
    if(is_writable(remote_path) == 0) {
        strcpy(server_message, "File is read only, which you have no right to delete");
        pthread_mutex_unlock(&file_operation_mutex);
        return;
    }

    if (remove(remote_path) == 0) {
        strcpy(server_message, "File deleted!");
    } else {
        strcpy(server_message, "Error deleting file!");
    }
    pthread_mutex_unlock(&file_operation_mutex);
}

