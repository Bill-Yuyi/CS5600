/*
 * server.c -- TCP Socket Server
 * 
 * adapted from: 
 *   https://www.educative.io/answers/how-to-implement-tcp-sockets-in-c
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "file_io.h"
#include "handle_request.h"
#include <pthread.h>
#include "global_mutex.h"

#define MAX_CLIENTS 100

void* request_handler(void *socket_desc) {
    int read_size, sock = *(int *) socket_desc;
    char server_message[8196], client_message[8196];
    char *command;
    char *remote_path;
    long file_size;
    char *rest;
    // Clean buffers:
    memset(server_message, '\0', sizeof(server_message));
    memset(client_message, '\0', sizeof(client_message));
    if ((read_size = recv(sock, client_message, sizeof(client_message), 0)) > 0) {
        rest = client_message;
        command = strtok_r(rest, "|", &rest);

        if (strcmp(command, "WRITE") == 0) {
            remote_path = strtok_r(rest, "|", &rest);
        } else {
            remote_path = rest;
        }

        if (strcmp(command, "WRITE") == 0) {
            int permission = is_writable(remote_path);
            if(permission != 2) {
                write_request(rest, remote_path, server_message);
            } else {
                write_request_with_permission(rest, remote_path,server_message);
            }

        } else if (strcmp(command, "GET") == 0) {
            get_request(remote_path, file_size, server_message);
        } else {
            remove_request(remote_path, server_message);
        }

        send(sock, server_message, strlen(server_message), 0);
    }
    if(read_size == 0) {
        puts("Client disconnected");
        fflush(stdout);
    } else if(read_size == -1) {
        perror("recv failed");
    }

    // Free the socket pointer
    close(sock);
    free(socket_desc);

    return 0;

}

int main(void)
{
    initialize_mutex();
    int socket_desc, client_sock, *new_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_size = sizeof(client_addr);

    // Create socket:
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
  
  if(socket_desc < 0){
    printf("Error while creating socket\n");
    return -1;
  }
  printf("Socket created successfully\n");
  
  // Set port and IP:
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(2000);
  server_addr.sin_addr.s_addr = INADDR_ANY;
  
  // Bind to the set port and IP:
  if(bind(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr))<0){
    printf("Couldn't bind to the port\n");
    return -1;
  }
  printf("Done with binding\n");

  // Listen for clients:
  if(listen(socket_desc, MAX_CLIENTS) < 0){
        printf("Error while listening\n");
        close(socket_desc);
        return -1;
  }
  printf("\nListening for incoming connections.....\n");

    while ((client_sock = accept(socket_desc, (struct sockaddr *)&client_addr, &client_size))) {
        printf("Connection accepted from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        pthread_t sniffer_thread;
        new_sock = malloc(1);
        *new_sock = client_sock;

        if (pthread_create(&sniffer_thread, NULL, request_handler, (void*) new_sock) < 0) {
            perror("could not create thread");
            return 1;
        }

        // Now join the thread, so that we don't terminate before the thread
        // pthread_join(sniffer_thread, NULL);
    }
  
  // Closing the socket:
  close(client_sock);
  close(socket_desc);
  destroy_mutex();
  return 0;
}
