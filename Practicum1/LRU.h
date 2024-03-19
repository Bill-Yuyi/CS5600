//
// Created by Yuyi Wang on 3/19/24.
//
#include "node.h"

typedef struct {
    int capacity;
    int count;
    node *head;
    node *tail;
    node *map;
} LRUCache;

LRUCache* initializeLRU(int capacity);
message_t* LRUCacheGet(LRUCache* cache, int key);
int LRUCacheFind(LRUCache* cache, int key);
void LRUCachePut(LRUCache* cache, message_t* messageToPut);
void destroyLRUCache(LRUCache *cache);
