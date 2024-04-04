//
// Created by Yuyi Wang on 4/3/24.
//

#ifndef GLOBAL_MUTEX_H
#define GLOBAL_MUTEX_H

#include <pthread.h>

extern pthread_mutex_t file_operation_mutex;

void initialize_mutex(void);
void destroy_mutex(void);

#endif
