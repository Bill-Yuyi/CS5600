#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct
{
    int *nums;
    int size;

} dynBlock;

dynBlock *allocDynBlock(int size)
{
    dynBlock *res = (dynBlock *)malloc(sizeof(dynBlock));
    if (res == NULL)
    {
        return NULL;
    }

    res->nums = (int *)malloc(size * (sizeof(int)));
    if (res->nums == NULL)
    {
        free(res);
        return NULL;
    }

    res->size = size;
    return res;
}

void storeMem2Blk(int *array, dynBlock *block, int size)
{
    if (block == NULL || block->nums == NULL)
    {
        return;
    }
    size = (size < block->size) ? size : block->size;
    for (int i = 0; i < size; i++)
    {
        block->nums[i] = array[i];
    }
}

int countNumbersInLine(const char *line)
{
    int count = 0;
    const char *temp = line;
    while (*temp)
    {
        if (isdigit(*temp))
        {
            count++;
            while (isdigit(*temp) || *temp == '.' || *temp == '-')
                temp++;
        }
        else
        {
            temp++;
        }
    }
    return count;
}

void fillArrayWithInts(const char *line, int *array, int maxCount)
{
    int count = 0;
    const char *delim = " ";
    char lineCopy[1024];
    strcpy(lineCopy, line);
    char *token = strtok(lineCopy, delim);

    while (token != NULL && count < maxCount)
    {
        array[count++] = atoi(token);
        token = strtok(NULL, delim);
    }
}

int main()
{
    FILE *file = fopen("blocks.data", "r");
    char line[1024];
    dynBlock *blocks[11];
    int count, *tempArray, i;

    if (file == NULL)
    {
        perror("Error opening file");
        return -1;
    }
    i = 0;

    while (fgets(line, sizeof(line), file) != NULL)
    {
        char lineCopy[1024];
        strcpy(lineCopy, line);
        count = countNumbersInLine(line);
        tempArray = (int *)malloc(count * sizeof(int));
        fillArrayWithInts(line, tempArray, count);

        blocks[i] = allocDynBlock(count);
        storeMem2Blk(tempArray, blocks[i], count);
        printf("block: ");
        for (int j = 0; j < blocks[i]->size; j++)
        {
            printf("%d ", blocks[i]->nums[j]);
        }
        printf("\n");
        i++;
        printf("line: %s\n", lineCopy);

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
