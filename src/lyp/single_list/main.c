#include <stdio.h>
#include <stdlib.h>

#include "single.h"


enum OPT {
    INSERT = 1,
    SELECT,
    DELETE,
    PRINTLIST,
    DESTROY,
    EXIT,
};

int main(void)
{
    struct single_list *tmp, *node, *sel, *del;
    struct single_list *head;
    int input;
    int val;


    create(&head);

    while(1){
        printf("1:insert, 2:select, 3:delete, 4:printlist, 5:destroy, 6:exit\n");
        scanf("%d", &input);

        switch(input){
        case INSERT :
            printf("The value of insert node : ");
            scanf("%d", &val);    
            node = (struct single_list *)malloc(sizeof(*node));
            node->date.no = val;
            insert_node(node, head);
            break;
        case SELECT : 
            printf("The value of select node : ");
            scanf("%d", &val);
            sel = select_node(val, head);
            if(sel != NULL)
                printf("success\n");
            else
                printf("failure\n");
            break;
        case DELETE :
            printf("The value of del node : ");
            scanf("%d", &val);
            del = delete_node(val, head);
            if(del != NULL){
                printf("success\n");
                free(del);
            }else{
                printf("there is no the value in the list\n");
            }
            break;
        case PRINTLIST:
            printf("the list is :");
            each_list(head, tmp)
                printf("%d ", tmp->date.no);
            putchar('\n');
            break;
        case DESTROY :
            delete_all_node(head);   
        case EXIT :
            printf("exit......\n");
            goto out;
        }
    }


out:
    delete_all_node(head);
    free(head);
    return 0;
}
