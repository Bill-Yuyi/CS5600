//
// Created by Yuyi Wang on 3/17/24.
//
#include "message.h"
#include "uthash.h"
typedef struct node
{
    /* data */
    int key;
    message_t *val;
    struct node *next;
    struct node *prev;
    UT_hash_handle hh;
}node;
