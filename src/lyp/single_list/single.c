#include <stdio.h>
#include <stdlib.h>


#include "single.h"


struct single_list *delete_node(int value, struct single_list *head)
{
    struct single_list *prev, *pos;

    each_list_safe_del(head, pos, prev){
        if(pos->date.no == value){
            prev->next = pos->next;
            return pos;
        }
    }

    return NULL;
}

void delete_all_node(struct single_list *head)
{
    struct single_list *tmp;

    if(head->next == NULL)
        return;

    for(tmp = head->next; tmp; tmp = head->next){
        head->next = tmp->next;
        free(tmp);
    }


}

struct single_list *select_node(int value, struct single_list *head)
{
    struct single_list *tmp;

    each_list(head ,tmp){
        if(tmp->date.no == value)
            return tmp;
    }
    return NULL;
}

static void insert_tail_node(struct single_list *end, struct single_list *node)
{
    end->next = node;
    node->next = NULL;
}

static void insert_prev_node(struct single_list *prev, struct single_list *node)
{
    node->next = prev->next;
    prev->next = node;
}


void insert_node(struct single_list *node, struct single_list *head)
{
    struct single_list *tmp;

    each_list(head, tmp){
        if(tmp->next == NULL){
            insert_tail_node(tmp, node);
            break;
        }
        else if(tmp->next->date.no >= node->date.no){
            insert_prev_node(tmp, node);
            break;
        }
    }
}



//struct single_list *create(struct single_list *head)
void create(struct single_list **list_head)
{
    FILE *fp;
    char buf[10] = {0};
    char const *path = "./single.txt";
    struct single_list *head;

    head = (struct single_list *)malloc(sizeof(*head));
    head->date.no = -1;
    head->next = NULL;

    fp = fopen(path, "r");
    while(fgets(buf, sizeof(buf), fp) != NULL){
        struct single_list *pos;
        int date_tmp;

        date_tmp = atoi(buf);
        pos = (struct single_list *)malloc(sizeof(*pos));
        pos->date.no = date_tmp;
        insert_prev_node(head, pos);
    }

    *list_head = head;
    //return head;
}
