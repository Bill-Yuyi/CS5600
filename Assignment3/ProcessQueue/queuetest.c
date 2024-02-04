#include "queue.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main()
{
    queue_t *queue = (queue_t *)malloc(sizeof(queue_t));
    initializeQueue(queue);

    // insert 5 process with distinct priority
    for (int i = 1; i <= 5; i++)
    {
        char name[10];
        char text[] = "no.";
        process_t *p = (process_t *)malloc(sizeof(process_t));
        sprintf(name, "%s%d", text, i);

        p->name = (char *)malloc(strlen(name) + 1);
        strcpy(p->name, name);

        p->identifier = i;
        p->priority = i;
        p->cumulativeRunTime = 1000 + i;
        add2q(queue, p);
    }

    // test they are correctly inserted
    printQueue(queue);
    printf("expected length: 5, actual: %d\n", qsize(queue));
    printf("----------\n");

    // test pop out the process with max priority
    printf("pop the max priroty\n");
    process_t *maxPriorityProcess = rmProcess(queue);
    printf("Process that is poped out: process %s\n", maxPriorityProcess->name);
    printQueue(queue);
    printf("expected length: 4, actual: %d\n", qsize(queue));
    printf("----------\n");
    free(maxPriorityProcess->name);
    free(maxPriorityProcess);

    // test pop out the front of queue
    printf("pop the front\n");
    process_t *processPopedOut = (process_t *)(popQ(queue));
    printf("Process that is poped out: process %s\n", processPopedOut->name);
    printQueue(queue);
    free(processPopedOut->name);
    free(processPopedOut);

    finishQueue(queue);
    return 0;
}