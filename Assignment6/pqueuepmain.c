#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

#define BUF_SIZE 15
#define MAX_ITEMS 30

int buffer[BUF_SIZE];
int num = 0;
int totalProduced = 0;
int totalConsumed = 0;

pthread_mutex_t mut;
pthread_cond_t cond;

void *producer(void *param);
void *consumer(void *param);

int main (int argc, char *argv[])
{
    pthread_t t1, t2, t3, t4;

    if(pthread_mutex_init(&mut, NULL) != 0) {
        perror("pthread_mutex_init");
        exit(1);
    }
    if(pthread_cond_init(&cond, NULL) != 0) {
        perror("pthread_cond_init");
        exit(1);
    }

    if (pthread_create(&t1, NULL, producer, NULL) != 0 ||
            pthread_create(&t3, NULL, producer, NULL) != 0) {
        perror("pthread_create");
        exit (1);
    }

    if (pthread_create(&t2, NULL, consumer, NULL) != 0
    || pthread_create(&t4, NULL, consumer, NULL) != 0) {
        perror("pthread_create");
        exit (1);
    }

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
    pthread_join(t4, NULL);
    printf("WE ARE DONE\n");
}

void *producer(void *param) {
    int i = 0;
    while(1) {
        pthread_mutex_lock(&mut);

        // Check if production limit reached
        if(totalProduced >= MAX_ITEMS) {
            pthread_mutex_unlock(&mut);
            printf("Producer exiting: produced %d\n", totalProduced);
            pthread_cond_broadcast(&cond); // Wake up all waiting consumers
            pthread_exit(NULL);
        }

        if (num >= BUF_SIZE) {
            pthread_cond_wait(&cond, &mut);
        }

        // Double check after waiting
        if(totalProduced >= MAX_ITEMS) {
            pthread_mutex_unlock(&mut);
            pthread_exit(NULL);
        }

        buffer[num] = i;
        num++;
        totalProduced++;
        printf("Producer produced: %d, Buffer size now: %d, total produced: %d\n", i, num, totalProduced);

        i++;

        pthread_cond_broadcast(&cond);
        pthread_mutex_unlock(&mut);
        sleep(1);
    }
}


void *consumer(void *param)
{
    int i=0;
    while (1) {
        pthread_mutex_lock (&mut);
        if(totalConsumed >= MAX_ITEMS) {
            pthread_mutex_unlock(&mut);
            printf("consumer exiting, consumed: %d\n", totalConsumed);
            pthread_exit(NULL);
        }

        if (num < 0) {
            exit(1);
        }

        while (num == 0){
            pthread_cond_wait (&cond, &mut);
        }
        i = buffer[num - 1];
        num--;
        totalConsumed++;
        printf("Consumer consumed: %d, Buffer size now: %d\n", i, num);
        pthread_mutex_unlock (&mut);
        pthread_cond_signal (&cond);
        sleep(3);
    }
}

