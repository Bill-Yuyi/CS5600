//
// Created by Yuyi Wang on 3/20/24.
//
#include <stdlib.h>
#include "message.h"

#define MEMORY_SIZE 513

typedef struct {
    message_t* messageMemory[MEMORY_SIZE];
}randomMemory;

randomMemory* initializeMemory() {
    randomMemory* memory = (randomMemory*) malloc(sizeof(randomMemory));
    for(int i = 0; i < MEMORY_SIZE; i++) {
        memory->messageMemory[i] = NULL;
    }

    return memory;
}

int findMessage(randomMemory* memory, int identifier) {
    for(int i = 0;i < MEMORY_SIZE; i++) {
        if(memory->messageMemory[i] != NULL && memory->messageMemory[i]->identifier == identifier) {
            return 1;
        }
    }
    return 0;
}

void putIntoMemory(randomMemory* memory, int identifier) {
    if(findMessage(memory, identifier) == 1) {
        return;
    }
    int randNumber = 1 + rand() % MEMORY_SIZE;
    message_t* result = retrieveMsg(identifier);
    if(result == NULL) {
        return;
    }
    memory->messageMemory[randNumber] = result;
}

void destroyMemory(randomMemory* memory) {
    for(int i = 0; i < MEMORY_SIZE; i++) {
        if(memory->messageMemory[i] != NULL) {
            destroyMessage(memory->messageMemory[i]);
        }
    }
    free(memory);
}