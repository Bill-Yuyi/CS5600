#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * A dynamic block structure that stores an array of integers.
 *
 */
typedef struct
{
    int *nums;
    int size;

} dynBlock;

/**
 * Allocates memory for a dynBlock.
 *
 * @param size The number of integers the array should hold.
 * @return A pointer to the newly allocated dynBlock, or NULL if memory allocation fails.
 */
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

/**
 * Stores an array of integers in a dynBlock.
 *
 * @param array The array of integers to be copied.
 * @param block The dynBlock in which to store the integers.
 * @param size The number of integers in the array to be copied.
 */
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