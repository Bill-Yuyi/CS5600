#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "dynblock.h"
#include "file_loader.h"

int main()
{
    FILE *file = fopen("blocks.data", "r");
    char line[1024];
    dynBlock *blocks[11];
    int count, i, *tempArray;

    if (file == NULL)
    {
        perror("Error opening file");
        return -1;
    }
    i = 0;

    while (fgets(line, sizeof(line), file) != NULL)
    {
        load2block(count, line, tempArray, i, blocks);
        printf("block: ");
        for (int j = 0; j < blocks[i]->size; j++)
        {
            printf("%d ", blocks[i]->nums[j]);
        }
        printf("\n");
        i++;
        printf("line: %s\n", line);

        free(tempArray);
    }

    for (int i = 0; i < 10; i++)
    {
        free(blocks[i]->nums);
        free(blocks[i]);
    }
    fclose(file);

    return 0;
}
