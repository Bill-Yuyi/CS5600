//
// Created by Yuyi Wang on 3/17/24.
//
#include <time.h>

typedef struct{
    int identifier;
    time_t timeStamp;
    char* sender;
    char* receiver;
    char* content;
    int flag;
} message_t;

message_t* createMessage(char* sender, char* receiver, char* context);
int storeMessage(message_t* msg);
message_t* retrieveMsg(int identifier);
void destroyMessage(message_t* msg);