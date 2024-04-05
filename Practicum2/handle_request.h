//
// Created by Yuyi Wang on 4/3/24.
//
int is_writable(const char* filename);
void write_request(char* rest, char* remote_path, char server_message[]);
void get_request(char* remote_path, long file_size, char server_message[]);
void remove_request(char* remote_path, char server_message[]);
void write_request_with_permission(char* rest, char* remote_path,char server_message[]);