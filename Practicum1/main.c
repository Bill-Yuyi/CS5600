//
// Created by Yuyi Wang on 3/17/24.
//
#include <stdlib.h>
#include <stdio.h>
#include "LRU.h"

#define LRU_CAPACITY 1024
#define RANDOM_MIN 1
#define RANDOM_MAX 1000

int main() {
    int i;
    int randNumber;
    int cacheMiss = 0;
    int cacheHit = 0;
    char sender[128];
    char receiver[128];
    char content[752];
    strcpy(sender, "A");
    strcpy(receiver, "B");
    strcpy(content, "This is a Message");

    // create 1000 random messages
    for(i = 0; i < 1000; i++) {
        message_t* new_msg = createMessage(sender, receiver, content);
        if(new_msg == NULL) {
            return 1;
        }
        storeMessage(new_msg);
        destroyMessage(new_msg);
    }

    LRUCache* cache = initializeLRU(LRU_CAPACITY);
    for(i = 0; i < 1000; i++) {
        randNumber = RANDOM_MIN + rand() % RANDOM_MAX;
        if(LRUCacheFind(cache, randNumber) == 1) {
            cacheMiss++;
        }else{
            cacheHit++;
        }
        LRUCacheGet(cache, randNumber);
    }

    printf("cache miss: %d, cache hit: %d, ratio: %.2f%% \n", cacheMiss, cacheHit, cacheHit / 1000.0 * 100);
    destroyLRUCache(cache);


    return 0;
}