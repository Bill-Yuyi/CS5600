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
 *
 * @return Pointer to the initialized LRUCache.
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

/**
 * Adds a node to the front of the LRU cache's doubly linked list, just after the head.
 * @param nodeToAdd Pointer to the node to be added
 * @param LRUCache Pointer to the LRUCache.
 */
void add(node* nodeToAdd, LRUCache* cache) {
    struct node* after = cache->head->next;
    nodeToAdd->next = after;
    after->prev = nodeToAdd;
    cache->head->next = nodeToAdd;
    nodeToAdd->prev = cache->head;
}

/**
 * Removes a node from the LRU cache's doubly linked list
 * @param nodeToDelete Pointer to the node to be deleted
 */
void delete(node* nodeToDelete) {
    struct node* before = nodeToDelete->prev;
    struct node* after = nodeToDelete->next;
    before->next = after;
    after->prev = before;
}

/**
 * Moves a node to the front of the LRU cache to mark it as recently used.
 * @param nodeToUpdate Pointer to the node to be updated
 * @param LRUCache Pointer to the LRUCache.
 */
void update(node* nodeToUpdate, LRUCache* cache) {
    delete(nodeToUpdate);
    add(nodeToUpdate, cache);
}

/**
 * Inserts or updates a message in the LRU cache.
 * If the cache exceeds its capacity, the least recently used item is removed.
 * @param LRUCache Pointer to the LRUCache.
 * @param messageToPut Pointer to the message.
 *
 */
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

/**
 * Retrieves a message from the LRU cache by its identifier.
 * If the message is not found in the cache, it attempts to retrieve it from the disk and then adds it to the cache.
 * @param LRUCache Pointer to the LRUCache.
 * @param key an identifier for the message.
 *
 * @return the message with the same identifier
 */
message_t* LRUCacheGet(LRUCache* cache, int key) {
    node* result;
    HASH_FIND_INT(cache->map, &key, result);
    if(result == NULL) {
        //if not existing in cache, get it from disk,and add it to cache
        result = (node*) malloc(sizeof(node));
        if(result == NULL) {
            perror("Error occurs when allocating memory for LRUCacheGet");
            return NULL;
        }

        result->val = retrieveMsg(key);
        if(result->val == NULL) {
            return NULL;
        }
        LRUCachePut(cache, result->val);
        return result->val;
    }
    update(result, cache);
    return result->val;
}

/**
 * Checks if a message with a given identifier exists in the LRU cache.
 * @param LRUCache Pointer to the LRUCache.
 * @param key an identifier for the message.
 *
 * @return 0 if the message exists in the cache; 1 if not.
 */
int LRUCacheFind(LRUCache* cache, int key) {
    node* result;
    HASH_FIND_INT(cache->map, &key, result);
    if(result == NULL) {
        return 1;
    }
    return 0;
}



/**
 * free the memory allocated for each msg in the cache.
 *
 * @param cache Pointer to the cache.
 */
void destroyLRUCache(LRUCache *cache)
{
    if (cache == NULL)
    {
        return;
    }

    struct node *current = cache->head;
    while (current != NULL)
    {
        struct node *next = current->next;

        if (current->val != NULL)
        {
            message_t *msg = current->val;
            destroyMessage(msg);
        }

        free(current);
        current = next;
    }
    cache->head = NULL;
    cache->tail = NULL;
    free(cache);
}