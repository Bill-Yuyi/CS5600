#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include "genRand.h"

int main(int argc, char *argv[])
{
    char *filename = argv[1];

    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        printf("Error opening file\n");
        return -1;
    }

    // write 10000 random words into file
    writeRandomWords(file);

    // initialize the queue
    queue_t *queue = (queue_t *)malloc(sizeof(queue_t));
    initializeQueue(queue);

    // add words into queue
    char line[1024];
    file = fopen(argv[1], "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return -1;
    }
    addWordsToQueue(file, queue);

    // start encoding
    processWords(queue);

    finishQueue(queue);
    fclose(file);

    return 0;
}