#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"


int is_overflow(struct queue *queue)
{
    return queue->tail >= MAX_SIZE-1 ? 1 : 0;
}

int is_empty(struct queue *queue)
{
    return queue->tail == queue->head ? 1 : 0;
}

int enqueue(struct queue *queue, int val)
{
    if(!is_overflow(queue)){
        queue->data[queue->tail++] = val;
        return 0;
    }
    return 1;
}


int dequeue(struct queue *queue){
    if(!is_empty(queue)){
        return *(queue->data + queue->head++);
    }
    return -1;
}

int check_head(struct queue *queue)
{
    return !is_empty(queue) ? queue->data[queue->head] : -1;
}


static void init_queue(struct queue *queue)
{
    FILE *fp;
    char const *path = "./queue.txt";
    char buf[10];
    int val;

    fp = fopen(path, "r");
    
    while(fgets(buf, sizeof(buf), fp) != NULL){
        val = atoi(buf);
        enqueue(queue, val);
    }
    
    return;
}

struct queue *create_queue(){
    struct queue *queue;

    queue = (struct queue *)malloc(sizeof(*queue));

    queue->tail = queue->head = 0;

    memset(queue->data, 0, sizeof(queue->data));

    init_queue(queue);
    return queue;
}


