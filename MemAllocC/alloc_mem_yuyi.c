#include <stdio.h>
#include <stdlib.h>

// 5mb of static segment
static char static_mem[5 * 1024 * 1024];

int main(int argc, char const *argv[])
{
    /* code */
    // 10mb of heap memory
    char *heap_mem = (char *)malloc(10 * 1024 * 1024);
    if (heap_mem == NULL)
    {
        return 1;
    }
    // heap memory is allocated

    // 1mb of stack memory
    char stack_mem[1024 * 1024];

    // free heap memory before exiting
    free(heap_mem);
    // memory on the heap is returned to os
    // memory on the stack is returned to os
    // static memory is returned to os automatically when the program exits
    return 0;
}
/**
 * what happens at program exit
 * 1. heap memory should be freeed manually otherwise it will cause memory leak
 * 2. static memory will be automatically reclaimed by os when program exits
 * 3. stack memory will be automatically reclaimed when the program exits

*/
