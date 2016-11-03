#include <stdio.h>
#include <stdlib.h>

#include "stack.h"

enum OPT {
        POP = 1,
        PUSH,
        CHECK_TOP,
        IS_EMPTY,
        IS_OVERFLOW,
        DESTROY,
        EXIT,
};


int main(void)
{
    struct stack *stack;
    int input;
    int val;
    int push;


    create_prev(&stack);
    //create_curr(&stack);

    while(1){
        printf("1:pop, 2:push, 3:check_top, 4: is_empty, 5: overflow 6 :destroy, 7:exit\n");
        scanf("%d", &input);

        switch(input){
        case POP :
            printf("input val : ");
            scanf("%d", &val);
            stack->pop(stack, val);
            break;
        case PUSH :
            if((push = stack->push(stack, PUSH_MARK)) != -1)
                printf("push val : %d\n", push);
            break; 
        case CHECK_TOP:
            stack->check_top(stack);
            break;
        case IS_EMPTY :
            if(stack->is_empty(stack))
                printf("its empty stack");
            else
                printf("it is not empty stack");
            break;
        case  IS_OVERFLOW :
            if(stack->is_overflow(stack))
                printf("overflow\n");
            else
                printf("Not overflow\n");
            break;
        case DESTROY :
            destroy(&stack);
        case EXIT :
            printf("exit......\n");
            goto out;
        }
    }


out:
    free(stack);
    return 0;
}
