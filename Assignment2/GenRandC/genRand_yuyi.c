#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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
        if (!isdigit((unsigned char)*str))
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

    if (argc != 3 && argc != 4)
    {
        printf("Usage: %s <number> <filename> [-a]\n", argv[0]);
        return -1;
    }

    if (!isDigitString(argv[1]))
    {
        printf("Please give a valid number in the first argument");
        return -1;
    }

    int num = atoi(argv[1]);
    char *filename = argv[2];
    char *mode = argc == 4 && strcmp(argv[3], "-a") == 0 ? "a" : "w";

    FILE *file = fopen(filename, mode);
    if (file == NULL)
    {
        printf("Error opening file\n");
        return -1;
    }

    for (int i = 0; i < num; i++)
    {
        fprintf(file, "%d ", genRand(1, 100));
    }

    fprintf(file, "\n");

    fclose(file);
    return 0;
}