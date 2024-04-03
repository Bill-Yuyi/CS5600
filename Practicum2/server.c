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

int main(void)
{
  int socket_desc, client_sock;
  socklen_t client_size;
  struct sockaddr_in server_addr, client_addr;
  char server_message[8196], client_message[8196];
  char* command;
  char* remote_path;
  long file_size;
  char* file_content;
  char* rest;
  char* content;
  char* msg;

  // Clean buffers:
  memset(server_message, '\0', sizeof(server_message));
  memset(client_message, '\0', sizeof(client_message));
  
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
  if(listen(socket_desc, 1) < 0){
        printf("Error while listening\n");
        close(socket_desc);
        return -1;
  }
  printf("\nListening for incoming connections.....\n");

  while(1) {
      // Clean buffers:
      memset(server_message, '\0', sizeof(server_message));
      memset(client_message, '\0', sizeof(client_message));

      // Accept an incoming connection:
      client_size = sizeof(client_addr);
      client_sock = accept(socket_desc, (struct sockaddr*)&client_addr, &client_size);

      if (client_sock < 0){
          printf("Can't accept\n");
          close(socket_desc);
          close(client_sock);
          return -1;
      }
      printf("Client connected at IP: %s and port: %i\n",
             inet_ntoa(client_addr.sin_addr),
             ntohs(client_addr.sin_port));

      // Receive client's message:
      if (recv(client_sock, client_message,
               sizeof(client_message), 0) < 0){
          printf("Couldn't receive\n");
          close(socket_desc);
          close(client_sock);
          return -1;
      }

      rest = client_message;
      command = strtok_r(rest, "|", &rest);

      if(strcmp(command, "WRITE") == 0) {
          remote_path = strtok_r(rest, "|", &rest);
      }else {
          remote_path = rest;
      }

      if(strcmp(command, "WRITE") == 0) {
          write_request(rest, remote_path,server_message);
      }else if(strcmp(command, "GET") == 0) {
        get_request(remote_path, file_size,server_message);
      }else {
          if(remove(remote_path) == 0) {
              strcpy(server_message, "File deleted!");
          }else {
              perror("Error deleting file");
          }
      }

      if (send(client_sock, server_message, strlen(server_message), 0) < 0){
          printf("Can't send\n");
          close(socket_desc);
          close(client_sock);
          return -1;
      }
  }

  
  // Closing the socket:
  close(client_sock);
  close(socket_desc);
  
  return 0;
}
