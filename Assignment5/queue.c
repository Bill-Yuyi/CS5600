#include <stdlib.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>

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

void addWordsToQueue(FILE *file, queue_t *queue)
{
    char line[1024];
    const char *delimiters = " "; // 定义分割单词的分隔符，包括空格和换行符

    while (fgets(line, sizeof(line), file) != NULL)
    {
        char *token = strtok(line, delimiters); // 分割第一个单词
        while (token != NULL)
        {
            char *word = strdup(token); // 复制单词
            if (word == NULL)
            {
                fprintf(stderr, "Memory allocation failed\n");
                exit(EXIT_FAILURE);
            }
            add2q(queue, word);               // 将单词添加到队列中
            token = strtok(NULL, delimiters); // 继续分割剩余的字符串
        }
    }
}