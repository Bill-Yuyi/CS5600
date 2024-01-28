typedef struct
{
    int *nums;
    int size;
} dynBlock;
dynBlock *allocDynBlock(int size);
void storeMem2Blk(int *array, dynBlock *block, int size);
