#include <stdio.h>
#include <stdlib.h>

#include "queue.h"

enum OPT {
        ENQUEUE = 1,
        DEQUEUE,
        CHECK_HEAD,
        IS_EMPTY,
        IS_OVERFLOW,
        DESTROY,
        EXIT,
};


int main(void)
{
    struct queue *queue;
    int input;
    int val;
    int dq;
    int *p;


    queue = create_queue();

    

    while(1){
        printf("1:enequeue, 2:dequeue, 3:check_head, 4: is_empty, 5: overflow 6 :destroy, 7:exit\n");
        scanf("%d", &input);

        switch(input){
        case ENQUEUE :
            printf("queue val : ");
            scanf("%d", &val);
            printf(!enqueue(queue, val) ? "enqueue\n" : "overflow, don't enqueue\n");
            break;
        case DEQUEUE :
            printf((dq = dequeue(queue)) == -1 ? 
                   "empty queue\n" : "dequeue val : %d\n", dq);
            break; 
        case CHECK_HEAD:
            printf(check_head(queue) == -1 ? 
                   "empty queue\n" : "head value : %d\n", 
                   *(queue->data + queue->head));
            break;
        case IS_EMPTY :
            printf(is_empty(queue) ? 
                   "its empty stack\n" : "it is not empty stack\n");
            break;
        case  IS_OVERFLOW :
            printf(is_overflow(queue) ? 
                   "overflow\n" : "Not overflow\n");
            break;
        case DESTROY :
            free(queue);
            goto out;
        case EXIT :
            goto free_q;
        }
    }
free_q:
    free(queue);
    printf("exit......\n");
out:
    return 0;
}
