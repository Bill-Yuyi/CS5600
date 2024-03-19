//
// Created by Yuyi Wang on 3/16/24.
//
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// total size = 1024 bytes
// identifier = 4 bytes, timeStamp = 8bytes, flag = 4 bytes
// remaining space = 1024 - 4 - 4 - 8 = 1008 bytes
// As I assign sender and receiver to 128 bytes, and content will be
// 1008 - 256 = 752 bytes
#define MAX_SENDER_SIZE 128
#define MAX_RECEIVER_SIZE 128
#define MAX_CONTENT_SIZE 752

typedef struct{
    int identifier;
    time_t timeStamp;
    char sender[MAX_SENDER_SIZE];
    char receiver[MAX_RECEIVER_SIZE];
    char content[MAX_CONTENT_SIZE];
    int flag;
} message_t;

static int last_identifier = 0;

char* strCopy(const char* str) {
    if(str == NULL) {
        return NULL;
    }

    char* dup = malloc(strlen(str) + 1);
    if(dup != NULL) strcpy(dup, str);
    return dup;
}

char* trimNewline(char* line) {
    char* p = strchr(line, '\n');
    if (p) *p = '\0';
    return line;
}

message_t* createMessage(const char* sender, const char* receiver,const char* content) {
    time_t currentTime = time(NULL);
    if(currentTime == ((time_t) -1)) {
        return NULL;
    }

    message_t* newMessage = (message_t *) malloc(sizeof(message_t));
    if(newMessage == NULL) {
        return NULL;
    }
    newMessage->identifier = ++last_identifier;
    newMessage->timeStamp = currentTime;
    strncpy(newMessage->sender, sender, MAX_SENDER_SIZE);
    newMessage->sender[MAX_SENDER_SIZE - 1] = '\0';

    strncpy(newMessage->receiver, receiver, MAX_RECEIVER_SIZE);
    newMessage->receiver[MAX_RECEIVER_SIZE - 1] = '\0';

    strncpy(newMessage->content, content, MAX_CONTENT_SIZE);
    newMessage->content[MAX_CONTENT_SIZE - 1] = '\0';

    newMessage->flag = 0;

    return newMessage;
}

int storeMessage(message_t* msg) {
    char filename[256];
    snprintf(filename, sizeof(filename), "%d.txt", msg->identifier);

    FILE* file = fopen(filename, "w");
    if(!file) {
        perror("Error opening the file");
        return -1; //error happened
    }
    msg->flag = 1;
    fprintf(file,"%ld\n%s\n= %s\n%s\n%d\n"
            ,(long)msg->timeStamp, msg->sender, msg->receiver, msg->content, msg->flag);
    fclose(file);
    return 0; // success
}

message_t* retrieveMsg(int identifier) {
    char filename[256];
    snprintf(filename, sizeof(filename), "%d.txt", identifier);

    message_t* msg = (message_t*) malloc(sizeof(message_t));
    if(msg == NULL) {
        perror("Error when allocating memory for msg");
        return NULL;
    }

    FILE* file = fopen(filename, "r");
    if(!file) {
        free(msg);
        perror("Error when opening the file");
        return NULL;
    }

    msg->identifier = identifier;
    char line[1024];
    int counter = 0;
    while(fgets(line, sizeof(line), file) != NULL) {
        trimNewline(line);
        switch(counter) {
            case 0:
                msg->timeStamp = (time_t) atol(line); break;
            case 1:
                strncpy(msg->sender, line, MAX_SENDER_SIZE);
                msg->sender[MAX_SENDER_SIZE - 1] = '\0';
                break;
            case 2:
                strncpy(msg->receiver, line, MAX_RECEIVER_SIZE);
                msg->receiver[MAX_RECEIVER_SIZE - 1] = '\0';
                break;
            case 3:
                strncpy(msg->content, line, MAX_CONTENT_SIZE);
                msg->content[MAX_CONTENT_SIZE - 1] = '\0';
                break;
            case 4:
                msg->flag = atoi(line); break;
        }
        counter++;
    }
    fclose(file);
    return msg;
}

void destroyMessage(message_t* msg) {
    if (msg != NULL) {
        free(msg);
    }
}