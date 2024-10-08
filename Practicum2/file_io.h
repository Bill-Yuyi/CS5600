//
// Created by Yuyi Wang on 4/2/24.
//
#include <stdio.h>


int write_file(const char* file_path, const char* content, size_t size);
char* read_file(const char* filename, long* size);
void set_permission(const char* remote_path, const char* permission);
int write_file_without_encode(const char* file_path, const char* content, size_t size);