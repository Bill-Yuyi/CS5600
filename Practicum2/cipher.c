#include <stdio.h>
#include "table.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "polybius.h"

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("Please provide at least 2 args\n");
        return -1;
    }

    if (strcmp(argv[1], "-e") != 0 && strcmp(argv[1], "-d"))
    {
        printf("Usage: %s <-a/-e> <plaintext> \n", argv[0]);
        return -1;
    }

    table_t *table = (table_t *)malloc(sizeof(table_t));
    if (table == NULL)
    {
        return -1;
    }
    initializeTable(table);

    size_t totalLength = 0;
    for (int i = 1; i < argc; i++)
    {
        totalLength += strlen(argv[i]);
    }

    char *res = (char *)malloc(totalLength * sizeof(char *));
    if (strcmp(argv[1], "-e") == 0)
    {
        for (int i = 2; i < argc; i++)
        {
            strcat(res, pbEncode(argv[i], table));
            strcat(res, " ");
        }
    }
    else
    {
        for (int i = 2; i < argc; i++)
        {
            strcat(res, pbDecode(argv[i], table));
            strcat(res, " ");
        }
    }
    printf("%s\n", res);

    free(table);
    free(res);
    return 0;
}