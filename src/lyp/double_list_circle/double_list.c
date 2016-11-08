#include<stdio.h>
#include <stdlib.h>

#include "double_list.h"

void delete_node(struct double_list *pos)
{
    struct double_list *prev = pos->prev, *next = pos->next;

    next->prev = prev;
    prev->next = next;

    free(pos);
}

void destroy_list(struct double_list *head)
{
    struct double_list *pos;

    if(head->next == head && head->prev == head)
        return;

    each_double_circle_list(head, pos)
        delete_node(pos);
}

int select_node(struct double_list *head, int val)
{
    struct double_list *pos;

    each_double_circle_list(head, pos)
        if(pos->data.value == val){
            printf("there is such data in the list : %d\n", val);
            return 0;
    }
    
    return 1;
}

/*
 * node1 ----- tail ----- node2
 * ===>
 * node1 ----- pos ----- tail ----- node2
 */
void insert_tail_node(struct double_list *tail, struct double_list *pos)
{
    tail->prev->next = pos;
    pos->prev = tail->prev;

    pos->next = tail;
    tail->prev = pos;

}

/*
 * node1 --- prev ---- node2
 * =====>
 * node1-----prev ---- pos ----- node2
 */ 
void insert_prev_node(struct double_list *prev, struct double_list *pos)
{
    prev->next->prev = pos;
    pos->next = prev->next;

    pos->prev = prev;
    prev->next = pos;
}

static void init_list(struct double_list *head)
{
    char const *path = "./double_list.txt";
    char buf[10];
    FILE *fp;

    fp = fopen(path, "r");
    while(fgets(buf, sizeof(buf), fp) != NULL){
        struct double_list *pos;
        int data_tmp;
        
        data_tmp = atoi(buf);
        pos = (struct double_list *)malloc(sizeof(*pos));
        pos->data.value = data_tmp;
        insert_tail_node(head, pos);
/*
        each_double_circle_list(head, pos)
            printf("head next : %d\n", pos->data.value);
        putchar('\n');
*/
    }
}

void create_circle(struct double_list **head)
{
    struct double_list *dl;

    dl = (struct double_list *)malloc(sizeof(*dl));

    dl->data.value = -1;

    dl->prev = dl;
    dl->next = dl;
   
    init_list(dl);
    
    *head = dl;
}


