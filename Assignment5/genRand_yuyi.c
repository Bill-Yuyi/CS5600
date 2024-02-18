#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "queue.h"
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

#define WORDS_PER_FILE 100

unsigned long lcg(unsigned long *seed)
{
    const unsigned long a = 22695477;
    const unsigned long c = 1;
    const unsigned long m = 2147483648;

    *seed = (a * (*seed) + c) % m;
    return *seed;
}

int genRand(int min, int max)
{
    static unsigned long seed = 0;
    int wordsPerLine = 20;
    if (seed == 0)
    {
        seed = time(NULL);
    }

    return (lcg(&seed) % (max - min + 1)) + min;
}

void processWords(queue_t *queue)
{
    int count = 0;
    int fd[2];
    pid_t pid;
    char *word;

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
                char filename[64];
                sprintf(filename, "./outputs/encrypted_%d.txt", count / WORDS_PER_FILE);
                execl("./cipher_program", "cipher_program", filename, NULL);
                exit(EXIT_SUCCESS);
            }
            close(fd[0]);
        }
        write(fd[1], word, strlen(word));
        write(fd[1], "\n", 1);
        count++;
        // printf("%d\n", qsize(queue));
    }

    close(fd[1]);
    // wait(NULL);
}

int main(int argc, char *argv[])
{
    unsigned long seed = time(NULL);
    int wordLength;
    int wordsPerLine = 20;

    char *filename = argv[1];

    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        printf("Error opening file\n");
        return -1;
    }

    for (int i = 0; i < 10000; i++)
    {
        char word[6];
        wordLength = genRand(1, 5);
        for (int j = 0; j < wordLength; j++)
        {
            word[j] = (char)(genRand(97, 122));
        }
        word[wordLength] = '\0';
        fprintf(file, "%s", word);
        if (i < 9999)
        {
            if ((i + 1) % wordsPerLine == 0)
            {
                fprintf(file, "\n");
            }
            else
            {
                fprintf(file, " ");
            }
        }
    }

    fclose(file);

    queue_t *queue = (queue_t *)malloc(sizeof(queue_t));
    initializeQueue(queue);

    char line[1024];

    file = fopen(argv[1], "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return -1;
    }

    addWordsToQueue(file, queue);
    printf("%d \n", qsize(queue));

    // start encoding
    processWords(queue);

    finishQueue(queue);
    fclose(file);

    return 0;
}