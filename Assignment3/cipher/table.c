#define CHAR_MAP_SIZE 5
#define ENCODE_SIZE 25
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct
{
    /* data */
    char charMap[CHAR_MAP_SIZE][CHAR_MAP_SIZE];
    int encodeMap[ENCODE_SIZE];
} table_t;

void initializeTable(table_t *table)
{
    const char letters[CHAR_MAP_SIZE * CHAR_MAP_SIZE] = {
        'A', 'B', 'C', 'D', 'E',
        'F', 'G', 'H', 'I', 'K',
        'L', 'M', 'N', 'O', 'P',
        'Q', 'R', 'S', 'T', 'U',
        'V', 'W', 'X', 'Y', 'Z'};
    int index = 0;
    for (int i = 0; i < CHAR_MAP_SIZE; i++)
        for (int j = 0; j < CHAR_MAP_SIZE; j++)
        {
            table->charMap[i][j] = letters[index];
            table->encodeMap[letters[index] - 'A'] = (i + 1) * 10 + j + 1;
            index++;
        }
    table->encodeMap['J' - 'A'] = 25;
}

char *encodeChar(const char *char2encode, table_t *table)
{

    char upperChar = *char2encode;
    if (*char2encode >= 'a' && *char2encode <= 'z')
    {
        upperChar -= 'a' - 'A';
    }

    char *res = malloc(3);
    if (res == NULL)
    {
        return NULL;
    }

    sprintf(res, "%02d", table->encodeMap[upperChar - 'A']);
    return res;
}
