#include <stdlib.h>
#include <limits.h>

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

void initializeQueue(queue_t *q)
{

    q->head = (struct node *)malloc(sizeof(struct node));
    if (q->head == NULL)
    {
        exit(1);
    }

    q->tail = (struct node *)malloc(sizeof(struct node));
    if (q->tail == NULL)
    {
        free(q->head);
        exit(1);
    }

    q->head->data = NULL;
    q->head->next = q->tail;
    q->head->prev = NULL;

    q->tail->data = NULL;
    q->tail->next = NULL;
    q->tail->prev = q->head;
}

void add2q(queue_t *queue, void *element)
{
    struct node *newNode = (struct * node) malloc(sizeof(struct node));
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
    node *tmp = queue->head->next;
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

int qsize(queue_t *queue)
{
    int size = 0;
    node *tmp = queue->head->next;
    while (tmp != queue->tail)
    {
        tmp = tmp->next;
        size++;
    }

    return size;
}