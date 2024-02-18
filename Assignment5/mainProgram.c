#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "queue.h"
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include "genRand.h"

#define WORDS_PER_FILE 100

void processWords(queue_t *queue)
{
    int count = 0;
    int fd[2];
    pid_t pid;
    char *word;

    // makdir outputs
    mkdir("./outputs", 0755);

    while ((word = (char *)popQ(queue)) != NULL)
    {
        if (count % WORDS_PER_FILE == 0)
        {
            if (pipe(fd) == -1)
            {
                perror("pipe");
                exit(EXIT_FAILURE);
            }

            pid = fork();
            if (pid == -1)
            {
                perror("fork");
                exit(EXIT_FAILURE);
            }

            if (pid == 0)
            {
                close(fd[1]);
                dup2(fd[0], STDIN_FILENO);
                close(fd[0]);

                char filename[64];
                sprintf(filename, "./outputs/encrypted_%d.txt", count / WORDS_PER_FILE);
                execl("./cipher_program", "cipher_program", filename, (char *)NULL);
                exit(EXIT_SUCCESS);
            }
            close(fd[0]);
        }

        write(fd[1], word, strlen(word));
        write(fd[1], "\n", 1);
        count++;

        if (count % WORDS_PER_FILE == 0)
        {
            close(fd[1]);
            waitpid(pid, NULL, 0);
        }
    }

    if (count % WORDS_PER_FILE != 0)
    {
        close(fd[1]);
        waitpid(pid, NULL, 0);
    }
}

int main(int argc, char *argv[])
{
    char *filename = argv[1];

    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        printf("Error opening file\n");
        return -1;
    }

    // write 10000 random words
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