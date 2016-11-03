#ifndef _QUEUE_H
#define _QUEUE_H




struct queue{
    int head;
    int tail;
    int data[20];
};

extern struct queue *create_queue();

int is_overflow(struct queue *s);
int is_empty(struct queue *s);
void enqueue(struct queue *s, int val);
int  dequeue(struct queue *s);
void check_head(struct queue *);


extern void destroy_queue(struct stack **);
#endif
