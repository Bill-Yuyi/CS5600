//
// Created by Yuyi Wang on 3/20/24.
//
#include "message.h"

#define MEMORY_SIZE 513
typedef struct {
    message_t* messageMemory[MEMORY_SIZE];
}randomMemory;

randomMemory* initializeMemory();
int findMessage(randomMemory* memory, int identifier);
void putIntoMemory(randomMemory* memory, int identifier);
void destroyMemory(randomMemory* memory);
