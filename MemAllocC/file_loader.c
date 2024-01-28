#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "dynblock.h"

/**
 * Counts the number of integers in a string.
 *
 * @param line The input string to be scanned for integers.
 * @return The count of integers found in the string.
 */
int countNumbersInLine(const char *line)
{
    int count = 0;
    const char *temp = line;
    while (*temp)
    {
        if (isdigit(*temp))
        {
            count++;
            while (isdigit(*temp))
                temp++;
        }
        else
        {
            temp++;
        }
    }
    return count;
}

/**
 * Fills an array with integers parsed from a string.
 *
 * @param line The input string containing integers.
 * @param array The array where integers will be stored.
 * @param maxCount The maximum number of integers to store in the array.
 */
void fillArrayWithInts(const char *line, int *array, int maxCount)
{
    int count = 0;
    const char *delimitor = " ";
    char lineCopy[1024];
    strcpy(lineCopy, line);
    char *token = strtok(lineCopy, delimitor);
    while (token != NULL && count < maxCount)
    {
        array[count++] = atoi(token);
        token = strtok(NULL, delimitor);
    }
}

/**
 * Loads integers from a string into a dynamic block.
 *
 * @param count The variable to store the count of integers in the string.
 * @param line The input string containing integers.
 * @param tempArray A pointer to an array for temporarily holding integers.
 * @param i The index at which the new dynBlock will be stored in the blocks array.
 * @param blocks An array of pointers to dynBlock structures.
 */
void load2block(int count, const char *line, int *tempArray, int i, dynBlock *blocks[])
{
    count = countNumbersInLine(line);
    tempArray = (int *)malloc(count * sizeof(int));
    fillArrayWithInts(line, tempArray, count);

    blocks[i] = allocDynBlock(count);
    storeMem2Blk(tempArray, blocks[i], count);
}