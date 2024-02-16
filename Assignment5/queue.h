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
void initializeQueue(queue_t *q);
void add2q(queue_t *queue, void *element);
void *popQ(queue_t *queue);
process_t *rmProcess(queue_t *queue);
int qsize(queue_t *queue);
void printQueue(queue_t *queue);
void finishQueue(queue_t *queue);
void addWordsToQueue(FILE *file, queue_t *queue);