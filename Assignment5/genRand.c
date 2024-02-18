#include <stdio.h>
#include <time.h>

#define WORDS_PER_LINE 20
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

void writeRandomWords(FILE *file)
{
    int wordLength;
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
            if ((i + 1) % WORDS_PER_LINE == 0)
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
}