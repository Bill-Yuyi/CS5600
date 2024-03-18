//
// Created by Yuyi Wang on 3/16/24.
//
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

typedef struct{
    int identifier;
    time_t timeStamp;
    char* sender;
    char* receiver;
    char* content;
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

message_t* createMessage(char* sender, char* receiver, char* context) {
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
    newMessage->sender = strCopy(sender);
    newMessage->receiver = strCopy(receiver);
    newMessage->content = strCopy(context);
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
                msg->sender = strCopy(line); break;
            case 2:
                msg->receiver = strCopy(line); break;
            case 3:
                msg->content = strCopy(line); break;
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
        free(msg->sender);
        free(msg->receiver);
        free(msg->content);
        free(msg);
    }
}