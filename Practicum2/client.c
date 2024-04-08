/*
 * client.c -- TCP Socket Client
 * 
 * adapted from: 
 *   https://www.educative.io/answers/how-to-implement-tcp-sockets-in-c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "file_io.h"


int main(int argc, char* argv[])
{
    int socket_desc;
    struct sockaddr_in server_addr;
    char* client_message;
    char server_message[2000];
    long file_size;
    char* content;
    char* remote_path;
    char* local_path;
    char* rest;
    char* permission;
    int message_len;
    int get_request_status;

    if(argc < 3) {
        if(argc != 2 || strcmp(argv[1], "STOP") == 1) {
            printf("Usage: %s WRITE local-file-path remote-file-path \n GET remote-file-path (local-file-path)\n "
                   "RM remote-file-path", argv[0]);
            return 1;
        }
    }

    // Clean buffers:
    memset(server_message,'\0',sizeof(server_message));

    char* command = argv[1];


    // read local file
    if(strcmp(command, "STOP") == 0) {
        message_len = strlen(command) + 20;

        client_message = malloc(message_len);
        if(client_message == NULL) {
            perror("Memory allocation for client message failed");
            return 1;
        }

        snprintf(client_message, message_len, "%s", command);
    }
    else if(strcmp(command, "WRITE") == 0) {
        local_path = argv[2];
        permission = NULL; // Initialize permission as NULL

        // If there are 5 arguments, the fourth is the remote path, and the fifth is the permission
        if(argc == 5) {
            remote_path = argv[3];
            permission = argv[4];
        } else if (argc == 4) { // If there are 4 arguments, the third could be either remote path or permission
            // Check if the third argument is "0" or "1", which means it's a permission
            if(strcmp(argv[3], "0") == 0 || strcmp(argv[3], "1") == 0) {
                permission = argv[3];
                remote_path = argv[2]; // Remote path defaults to local path if only permission is provided
            } else {
                remote_path = argv[3]; // If it's not a permission, treat it as a remote path
            }
        } else {
            remote_path = argv[2]; // Default to local path if no additional arguments are provided
        }

        if(permission != NULL && (strcmp(permission, "0") != 0 && strcmp(permission, "1") != 0)) {
            printf("Invalid permission. Usage: %s WRITE local-file-path remote-file-path [permission 0 or 1]\n", argv[0]);
            return 1;
        }

        content = read_file(local_path, &file_size);
        if(content == NULL) {
            perror("Memory allocation for file content failed");
            return 1;
        }

        // Calculate message length more accurately
        message_len = strlen(command) + strlen(remote_path) + strlen(content) + 40;
        if (permission != NULL) {
            message_len += strlen(permission);
        }

        client_message = malloc(message_len);
        if(client_message == NULL) {
            free(content);
            perror("Memory allocation for client message failed");
            return 1;
        }

        if(permission != NULL) {
            snprintf(client_message, message_len, "%s|%s|%ld|%s|%s", command, remote_path, file_size, content, permission);
        } else {
            snprintf(client_message, message_len, "%s|%s|%ld|%s", command, remote_path, file_size, content);
        }

        free(content); // Ensure content is freed after use
    }

    else if(strcmp(command, "GET") == 0) {
        remote_path = argv[2];

        char* file_name = strrchr(remote_path, '/');
        if (file_name) {
            file_name++;
        } else {
            file_name = remote_path;
        }

        local_path = (argc == 3) ? file_name : argv[3];

        message_len = strlen(command) + strlen(remote_path) + 20;
        client_message = malloc(message_len);
        if(client_message == NULL) {
            perror("Memory allocation for server message is failed");
            return 1;
        }
        snprintf(client_message, message_len,"%s|%s", command, remote_path);
    }
    else if(strcmp(command, "RM") == 0 || strcmp(command, "LS") == 0){
        remote_path = argv[2];
        message_len = strlen(command) + strlen(remote_path) + 20;
        client_message = malloc(message_len);
        if(client_message == NULL) {
            perror("Memory allocation for server message is failed");
            return 1;
        }
        snprintf(client_message, message_len,"%s|%s", command, remote_path);
    } else {
        perror("No such command!");
        return 1;
    }


    // Create socket:
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);

    if(socket_desc < 0){
        printf("Unable to create socket\n");
        close(socket_desc);
        return -1;
    }

    printf("Socket created successfully\n");

    // Set port and IP the same as server-side:
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(2000);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Send connection request to server:
    if(connect(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
        printf("Unable to connect\n");
        close(socket_desc);
        free(client_message);

        return -1;
    }
    printf("Connected with server successfully\n");

    // Get input from the user:
    //printf("Enter command: ");
    //gets(client_message);

    // Send the message to server:
    if(send(socket_desc, client_message, strlen(client_message), 0) < 0){
        printf("Unable to send message\n");
        close(socket_desc);
        free(client_message);
        return -1;
    }

    // Receive the server's response:
    if(recv(socket_desc, server_message, sizeof(server_message), 0) < 0){
        printf("Error while receiving server's msg\n");
        close(socket_desc);
        free(client_message);
        return -1;
    }

    if(strcmp(command, "GET") == 0) {

        if(strcmp(server_message, "No such file") != 0) {
            if(write_file_without_encode(local_path, server_message, strlen(server_message)) == 0) {
                printf("Content written to %s successfully\n", local_path);
            }else {
                printf("Failed to write content to %s\n", local_path);
            }
        }
    }

    //printf("Server's response: %s\n",server_message);

    // Close the socket:
    close(socket_desc);
    free(client_message);

    return 0;
}
