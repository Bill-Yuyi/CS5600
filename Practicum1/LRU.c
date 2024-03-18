//
// Created by Yuyi Wang on 3/17/24.
//
#include <stdio.h>
#include "uthash.h"
#include "node.h"

typedef struct {
    int capacity;
    int count;
    node *head;
    node *tail;
    node *map;
} LRUCache;

/**
 * Initializes a LRU by allocating memory for the head and tail nodes
 * and setting them up with no actual data.
 * @param LRUCache Pointer to the LRUCache to be initialized.
 */
LRUCache* initializeLRU(int capacity)
{
    LRUCache* cache = (LRUCache*)malloc(sizeof(LRUCache));
    if(cache == NULL) {
        perror("Error occurs when allocating memory for lru");
        return NULL;
    }
    cache->head = (node*)malloc(sizeof(node)); // Dummy head
    if(cache->head == NULL) {
        perror("Error occurs when allocating memory for head");
        return NULL;
    }
    cache->tail = (node*)malloc(sizeof(node)); // Dummy tail
    if(cache->tail == NULL) {
        perror("Error occurs when allocating memory for tail");
        return NULL;
    }

    cache->head->next = cache->tail;
    cache->tail->prev = cache->head;
    cache->capacity = capacity;
    cache->count = 0;
    cache->map = NULL; // Initialize the hashmap
    return cache;
}

void add(node* nodeToAdd, LRUCache* cache) {
    struct node* after = cache->head->next;
    nodeToAdd->next = after;
    after->prev = nodeToAdd;
    cache->head->next = nodeToAdd;
    nodeToAdd->prev = cache->head;
}

void delete(node* nodeToDelete) {
    struct node* before = nodeToDelete->prev;
    struct node* after = nodeToDelete->next;
    before->next = after;
    after->prev = before;
}

void update(node* nodeToUpdate, LRUCache* cache) {
    delete(nodeToUpdate);
    add(nodeToUpdate, cache);
}

message_t* LRUCacheGet(LRUCache* cache, int key) {
    node* result;
    HASH_FIND_INT(cache->map, &key, result);
    if(result == NULL) {
        //TODO: if not existing in cache, get it from disk,and add it to cache
        result = (node*) malloc(sizeof(node));
        if(result == NULL) {
            perror("Error occurs when allocating memory for LRUCacheGet");
            return NULL;
        }

        result->val = retrieveMsg(key);
        if(result->val == NULL) {
            return NULL;
        }
        add(result, cache);
        return result->val;
    }
    update(result, cache);
    return result->val;
}

void LRUCachePut(LRUCache* cache, message_t* messageToPut) {
    node* existingNode;
    HASH_FIND_INT(cache->map, &messageToPut->identifier, existingNode);
    if(existingNode != NULL) {
        existingNode->val = messageToPut;
        update(existingNode, cache);
    }else {
        node* newNode = (node*) malloc(sizeof(node));
        if(newNode == NULL) {
            perror("Error occurs when allocating memory for newNode in put");
            return;
        }

        int key = messageToPut->identifier;
        newNode->key = key;
        newNode->val = messageToPut;
        HASH_ADD_INT(cache->map, key, newNode);
        add(newNode, cache);

        cache->count++;
        if(cache->count > cache->capacity) {
            node* toRemove = cache->tail->prev;
            delete(toRemove);
            HASH_DEL(cache->map, toRemove);
            free(toRemove);
            cache->count--;
        }
    }
}