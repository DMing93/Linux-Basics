#ifndef _QUEUE_H
#define _QUEUE_H


#define MAX_SIZE 20


struct queue{
    int head;
    int tail;
    int data[20];
};

extern struct queue *create_queue();

extern int is_overflow(struct queue *s);
extern int is_empty(struct queue *s);
extern int enqueue(struct queue *s, int val);
extern int  dequeue(struct queue *s);
extern int check_head(struct queue *);


extern void destroy_queue(struct queue **);
#endif
