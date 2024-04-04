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
#include <pthread.h>
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
  int message_len;
  if(argc < 3) {
      printf("Usage: %s WRITE|GET|RM local-file-path remote-file-path\n GET remote-file-path (local-file-path)\n "
             "RM remote-file-path", argv[0]);
      return 1;
  }

  // Clean buffers:
  memset(server_message,'\0',sizeof(server_message));

  char* command = argv[1];


  // read local file
  if(strcmp(command, "WRITE") == 0) {
      local_path = argv[2];
      remote_path = argc == 3 ? argv[2] : argv[3];
      content = read_file(local_path, &file_size);
      if(content == NULL) {
          perror("memory allocation for file content is failed");
          return 1;
      }

      message_len = strlen(command) + strlen(remote_path) + file_size + 20; // 20 extra bytes for separators and null terminator
      client_message = malloc(message_len);
      if(client_message == NULL) {
          free(content);
          perror("Memory allocation for server message is failed");
          return 1;
      }
      snprintf(client_message, message_len,"%s|%s|%ld|%s", command, remote_path, file_size, content);
      free(content);
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
  else {
      remote_path = argv[2];
      message_len = strlen(command) + strlen(remote_path) + 20;
      client_message = malloc(message_len);
      if(client_message == NULL) {
          perror("Memory allocation for server message is failed");
          return 1;
      }
      snprintf(client_message, message_len,"%s|%s", command, remote_path);
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
  server_addr.sin_addr.s_addr = inet_addr("192.168.0.126");
  
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
      if(write_file(local_path, server_message, strlen(server_message)) == 0) {
          printf("Content written to %s successfully\n", local_path);
      }else {
          printf("Failed to write content to %s\n", local_path);
      }
  }
  
  printf("Server's response: %s\n",server_message);
  
  // Close the socket:
  close(socket_desc);
  free(client_message);

  return 0;
}
