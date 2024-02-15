#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "queue.h"

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

    if (seed == 0)
    {
        seed = time(NULL);
    }

    return (lcg(&seed) % (max - min + 1)) + min;
}

int isDigitString(const char *str)
{
    while (*str)
    {
        if (!isdigit((char)*str))
        {
            return 0;
        }
        str++;
    }
    return 1;
}

int main(int argc, char *argv[])
{
    unsigned long seed = time(NULL);
    int wordLength;

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
        fprintf(file, "%s ", word);
    }
    fprintf(file, "\n");

    queue_t *queue = (queue_t *)malloc(sizeof(queue_t));
    initializeQueue(queue);

    char line[1024];

    file = fopen("text.txt", "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return -1;
    }

    while (fgets(line, sizeof(line), file) != NULL)
    {
        add2q(queue, line);
    }

    free(queue);
    fclose(file);

    return 0;
}