#include <stdlib.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

#define WORDS_PER_FILE 100
typedef struct
{
    /* data */
    int identifier;
    char *name;
    long cumulativeRunTime;
    int priority;
} process_t;

struct node
{
    /* data */
    void *data;
    struct node *next;
    struct node *prev;
};

typedef struct
{
    /* data */
    struct node *head;
    struct node *tail;
} queue_t;

/**
 * Initializes a queue by allocating memory for the head and tail nodes
 * and setting them up with no actual data.
 * @param queue Pointer to the queue to be initialized.
 */
void initializeQueue(queue_t *queue)
{

    queue->head = (struct node *)malloc(sizeof(struct node));
    if (queue->head == NULL)
    {
        exit(1);
    }

    queue->tail = (struct node *)malloc(sizeof(struct node));
    if (queue->tail == NULL)
    {
        free(queue->head);
        exit(1);
    }

    queue->head->data = NULL;
    queue->head->next = queue->tail;
    queue->head->prev = NULL;

    queue->tail->data = NULL;
    queue->tail->next = NULL;
    queue->tail->prev = queue->head;
}

/**
 * Adds a new element to the end of the queue.
 *
 * @param queue Pointer to the queue where the element will be added.
 * @param element Pointer to the data to be added to the queue.
 */
void add2q(queue_t *queue, void *element)
{
    struct node *newNode = (struct node *)malloc(sizeof(struct node));
    if (newNode == NULL)
    {
        return;
    }

    newNode->data = element;
    newNode->next = queue->tail;
    newNode->prev = queue->tail->prev;

    queue->tail->prev->next = newNode;
    queue->tail->prev = newNode;
}

/**
 * Removes and returns the data from the first node in the queue.
 *
 * @param queue Pointer to the queue.
 * @return Void pointer to the data of the first node,
 * or NULL if the queue is empty.
 */
void *popQ(queue_t *queue)
{
    if (queue->head->next == queue->tail)
    {
        return NULL;
    }

    struct node *tmp = queue->head->next;

    queue->head->next = tmp->next;
    tmp->next->prev = queue->head;

    void *data = tmp->data;
    free(tmp);

    return data;
}

/**
 * Removes and returns the process with the highest priority from the queue.
 *
 * @param queue Pointer to the queue.
 * @return Pointer to the process_t structure with the highest priority,
 * or NULL if the queue is empty.
 */
process_t *rmProcess(queue_t *queue)
{
    // only dummy node return null
    if (queue->head->next == queue->tail)
    {
        return NULL;
    }

    // store the current highest priority node
    struct node *highPriorityNode = NULL;
    int highestPriority = INT_MIN;

    // pointer to traverse the queue
    struct node *tmp = queue->head->next;
    while (tmp != queue->tail)
    {
        process_t *currentProcess = (process_t *)(tmp->data);
        if (currentProcess->priority > highestPriority)
        {
            highestPriority = currentProcess->priority;
            highPriorityNode = tmp;
        }
        tmp = tmp->next;
    }
    if (highPriorityNode != NULL)
    {
        highPriorityNode->prev->next = highPriorityNode->next;
        highPriorityNode->next->prev = highPriorityNode->prev;
        process_t *process = (process_t *)(highPriorityNode->data);
        free(highPriorityNode);
        return process;
    }
    return NULL;
}

/**
 * Calculates and returns the number of data nodes in the queue.
 *
 * @param queue Pointer to the queue.
 * @return The number of data nodes in the queue as an integer.
 */
int qsize(queue_t *queue)
{
    int size = 0;
    struct node *tmp = queue->head->next;
    while (tmp != queue->tail)
    {
        tmp = tmp->next;
        size++;
    }

    return size;
}

/**
 * free the memory allocated for each process in the queue.
 *
 * @param queue Pointer to the queue.
 */
void finishQueue(queue_t *queue)
{
    if (queue == NULL)
    {
        return;
    }

    struct node *current = queue->head;
    while (current != NULL)
    {
        struct node *next = current->next;

        if (current->data != NULL)
        {
            process_t *process = (process_t *)current->data;
            free(process->name);
            free(process);
        }

        free(current);
        current = next;
    }
    queue->head = NULL;
    queue->tail = NULL;
    free(queue);
}

/**
 * Prints the priority and name of each process in the queue.
 *
 * @param queue Pointer to the queue.
 */
void printQueue(queue_t *queue)
{
    struct node *tmp = queue->head->next;
    while (tmp != queue->tail)
    {
        char *currentWord = (char *)(tmp->data);

        printf("%s \n", currentWord);
        tmp = tmp->next;
    }
}

/**
 * Read the file and add words into the queue
 *
 * @param file Pointer to the file.
 * @param queue Pointer to the queue.
 */
void addWordsToQueue(FILE *file, queue_t *queue)
{
    char line[1024];
    const char *delimiters = " ";

    while (fgets(line, sizeof(line), file) != NULL)
    {
        char *token = strtok(line, delimiters);
        while (token != NULL)
        {
            char *word = strdup(token);
            if (word == NULL)
            {
                fprintf(stderr, "Memory allocation failed\n");
                exit(EXIT_FAILURE);
            }
            add2q(queue, word);
            token = strtok(NULL, delimiters);
        }
    }
}

/**
 * Encode words in the queue in size of 100 words as a batch.
 *
 * @param queue Pointer to the queue.
 */
void processWords(queue_t *queue)
{
    int count = 0;
    int fd[2];
    pid_t pid;
    char *word;

    // makdir outputs
    mkdir("./outputs", 0755);

    while ((word = (char *)popQ(queue)) != NULL)
    {
        if (count % WORDS_PER_FILE == 0)
        {
            if (pipe(fd) == -1)
            {
                perror("pipe");
                exit(EXIT_FAILURE);
            }

            pid = fork();
            if (pid == -1)
            {
                perror("fork");
                exit(EXIT_FAILURE);
            }

            if (pid == 0)
            {
                close(fd[1]);
                dup2(fd[0], STDIN_FILENO);
                close(fd[0]);

                char filename[64];
                sprintf(filename, "./outputs/encrypted_%d.txt", count / WORDS_PER_FILE);
                execl("./cipherProgram", "cipherProgram", filename, (char *)NULL);
                exit(EXIT_SUCCESS);
            }
            close(fd[0]);
        }

        write(fd[1], word, strlen(word));
        write(fd[1], "\n", 1);
        count++;

        if (count % WORDS_PER_FILE == 0)
        {
            close(fd[1]);
            waitpid(pid, NULL, 0);
        }
    }

    if (count % WORDS_PER_FILE != 0)
    {
        close(fd[1]);
        waitpid(pid, NULL, 0);
    }
}