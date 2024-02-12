#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
/**
 * Answer to Question 4:
 * it is not consistent since thread 2 may access the array indecies where
 * thread 1 havent yet initialized. This inconsistency is expected when multiple threads access
 * and modify shared data without proper synchronization.
 * To avoid such problems, we should use something like mutexes, semaphores, or condition variables.
 *
 */
int globalArr[1000];
void *workerForTh1(void *data)
{

    for (int i = 0; i < 120; i++)
    {
        usleep(50000);
        globalArr[i] = i;
    }

    printf("Thread %s done!\n", "1");
    return NULL;
}

void *workerForTh2(void *data)
{

    for (int i = 0; i < 120; i++)
    {
        usleep(50000);
        printf("number in position %d is: %d\n", i, globalArr[i]);
    }

    printf("Thread %s done!\n", "2");
    return NULL;
}

int main(void)
{
    pthread_t th1, th2;
    pthread_create(&th1, NULL, workerForTh1, "X");
    pthread_create(&th2, NULL, workerForTh2, "Y");
    sleep(1);
    printf("====> Cancelling Thread %lu!\n", (unsigned long)th2);
    pthread_cancel(th2);
    usleep(100000);
    printf("====> Cancelling Thread %lu!\n", (unsigned long)th1);
    pthread_cancel(th1);
    printf("exiting from main program\n");
    return 0;
}