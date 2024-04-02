//
// Created by Yuyi Wang on 3/20/24.
//
#include <stdlib.h>
#include "message.h"

#define MEMORY_SIZE 1000

typedef struct {
    message_t* messageMemory[MEMORY_SIZE];
}randomMemory;

/**
 * Initializes a randomMemory by allocating memory for the message_t array
 *
 * @return Pointer to the initialized randomMemory.
 */
randomMemory* initializeMemory() {
    randomMemory* memory = (randomMemory*) malloc(sizeof(randomMemory));
    for(int i = 0; i < MEMORY_SIZE; i++) {
        memory->messageMemory[i] = NULL;
    }

    return memory;
}

/**
 * Checks if a message with a given identifier exists in the randomMemory cache.
 * @param randomMemory Pointer to the LRUCache.
 * @param identifier an identifier for the message.
 *
 * @return 0 if the message exists in the cache; 1 if not.
 */
int findMessage(randomMemory* memory, int identifier) {
    for(int i = 0;i < MEMORY_SIZE; i++) {
        if(memory->messageMemory[i] != NULL && memory->messageMemory[i]->identifier == identifier) {
            return 0;
        }
    }
    return 1;
}

/**
 * Retrieves a message from the random memory cache by its identifier.
 * If the message is not found in the cache,
 * it attempts to retrieve it from the disk and then adds it to the cache.
 * @param randomMemory Pointer to the randomMemory.
 * @param identifier an identifier for the message.
 *
 * @return the message with the same identifier
 */
void putIntoMemory(randomMemory* memory, int identifier) {
    if(findMessage(memory, identifier) == 1) {
        return;
    }
    int randNumber = rand() % MEMORY_SIZE;
    message_t* result = retrieveMsg(identifier);
    if(result == NULL) {
        return;
    }
    memory->messageMemory[randNumber] = result;
}

/**
 * free the memory allocated for each msg in the randomMemory.
 *
 * @param memory Pointer to the randomMemory.
 */
void destroyMemory(randomMemory* memory) {
    for(int i = 0; i < MEMORY_SIZE; i++) {
        if(memory->messageMemory[i] != NULL) {
            destroyMessage(memory->messageMemory[i]);
        }
    }
    free(memory);
}