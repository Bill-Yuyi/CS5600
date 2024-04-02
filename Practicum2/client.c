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

int main(int argc, char* argv[])
{
  int socket_desc;
  struct sockaddr_in server_addr;
  char* client_message;
  char server_message[2000];
  long file_size;

  if(argc < 3) {
      printf("Usage: %s WRITE local-file-path remote-file-path\n", argv[0]);
      return 1;
  }

  // Clean buffers:
  memset(server_message,'\0',sizeof(server_message));

  char* command = argv[1];
  char* local_path = argv[2];
  char* remote_path = argc == 3 ? argv[2] : argv[3];

  // read local file
  char* content = read_file(local_path, &file_size);
  if(content == NULL) {
      perror("memory allocation for file content is failed");
      return 1;
  }

  int message_len = strlen(command) + strlen(remote_path) + file_size + 20; // 20 extra bytes for separators and null terminator
  client_message = malloc(message_len);
  if(client_message == NULL) {
      free(content);
      perror("Memory allocation for server message is failed");
      return 1;
  }
  snprintf(client_message, message_len,"%s|%s|%ld|%s", command, remote_path, file_size, content);

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
    free(content);
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
      free(content);
    return -1;
  }


  // Receive the server's response:
  if(recv(socket_desc, server_message, sizeof(server_message), 0) < 0){
    printf("Error while receiving server's msg\n");
    close(socket_desc);
    free(client_message);
    free(content);
    return -1;
  }
  
  printf("Server's response: %s\n",server_message);
  
  // Close the socket:
  close(socket_desc);
  free(client_message);
  free(content);
  return 0;
}
