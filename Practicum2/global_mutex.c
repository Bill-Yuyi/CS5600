//
// Created by Yuyi Wang on 4/3/24.
//

#include "global_mutex.h"

pthread_mutex_t file_operation_mutex;

void initialize_mutex(void) {
    pthread_mutex_init(&file_operation_mutex, NULL);
}

void destroy_mutex(void) {
    pthread_mutex_destroy(&file_operation_mutex);
}