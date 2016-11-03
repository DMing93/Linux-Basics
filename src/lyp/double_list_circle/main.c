#include <stdio.h>
#include <stdlib.h>

#include "double_list.h"


enum OPT {
    INSERT = 1,
    SELECT,
    DELETE,
    PRINTLIST,
    DESTROY,
    EXIT,
};

static struct double_list *delete_opt(struct double_list *head, int val)
{
    struct double_list *pos;

    each_double_circle_list(head, pos)
        if(pos->data.value == val)
            return pos;

    return NULL;
}

static void insert_node(struct double_list *head, int val)
{
    struct double_list *pos;
    struct double_list *node;

    node = (struct double_list *)malloc(sizeof(*node));
    node->data.value = val;

    if(head->prev->data.value < val){
        insert_tail_node(head, node);
        return;
    }

    
    each_double_circle_list(head, pos)
        if(pos->data.value >= val)
            insert_prev_node(pos->prev, node);
 
}

int main(void)
{
    struct double_list *tmp, *sel, *del, *pos;
    struct double_list *head;
    int input;
    int val;


    create_circle(&head);


    while(1){
        printf("1:insert, 2:select, 3:delete, 4:printlist, 5:DELETE, 6:exit\n");
        scanf("%d", &input);

        switch(input){
        case INSERT :
            printf("The value of insert node : ");
            scanf("%d", &val);
            insert_node(head, val);
            break;
        case SELECT : 
            printf("The value of select node : ");
            scanf("%d", &val);
            if(select_node(head, val))
                printf("there is no data in the list\n");
            break;
        case DELETE :
            printf("The value of select node : ");
            scanf("%d", &val);
            del = delete_opt(head, val);
            if(del != NULL){
                delete_node(del);
                printf("delete success\n");
            }else{
                printf("there is no the value in the list\n");
            }
            break;
        case PRINTLIST:
            printf("the list is :");
            each_double_circle_list(head, tmp)
                printf("%d ", tmp->data.value);
            putchar('\n');
            break;
        case DESTROY :
            destroy_list(head);
        case EXIT :
            printf("exit......\n");
            goto out;
        }
    }

out:
    destroy_list(head);
    free(head);
    return 0;
}
