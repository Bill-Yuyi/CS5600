#include <stdio.h>
#include <time.h>

#define WORDS_PER_LINE 20

/**
 * Implements a lcg algorithm for generating pseudorandom numbers.
 *
 * @param seed A pointer to an unsigned long value representing the current seed.
 *
 * @return The pseudorandom number generated using the lcg algorithm.
 */
unsigned long lcg(unsigned long *seed)
{
    const unsigned long a = 22695477;
    const unsigned long c = 1;
    const unsigned long m = 2147483648;

    *seed = (a * (*seed) + c) % m;
    return *seed;
}

/**
 * Generates a random integer within the range [min, max] inclusive.
 *
 * @param min The minimum value of the range.
 * @param max The maximum value of the range.
 * @return A pseudorandom integer between min and max.
 */
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

/**
 * Writes randomly generated words to a file. Each word is composed of lowercase letters
 * with lengths in range [1,5].
 *
 * @param file The file pointer.
 */
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