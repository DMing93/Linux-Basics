#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "stack.h"


static int is_overflow_prev(struct stack *s)
{
    return s->top == STACK_TOP_PREV ? 1 : 0;
}

static int is_overflow_curr(struct stack *s)
{
    return s->top == STACK_TOP_CURR ? 1 : 0;
}

static int is_empty_curr(struct stack *s)
{
    return s->top == STACK_BOT_CURR ? 1 : 0;
}
static int is_empty_prev(struct stack *s)
{
    return s->top == STACK_BOT_PREV ? 1 : 0;
}

static void init_stack(struct stack *s)
{
    FILE *fp;
    char const *path = "./stack.txt";
    char buf[10];
    int val;

    fp = fopen(path, "r");
    
    while(fgets(buf, sizeof(buf), fp) != NULL){
        val = atoi(buf);
        s->pop(s, val);
    }

    return;
}

static void check_top_prev(struct stack *s)
{
    if(s->top <= STACK_BOT_PREV)
        printf("there is no data\n");
    else
        printf("top data is %d\n", s->data[s->top]);
}

static void pop_prev(struct stack *s, int val)
{
    if(s->top >= STACK_TOP_PREV){
        printf("overflow!\n");
        return;
    }
    s->data[++s->top] = val;
    return;
}

static int push_prev(struct stack *s, int mark)
{
    if(s->top <= STACK_BOT_PREV){
        if(!mark)
            printf("there is no data\n");
        return -1;
    }
    return s->data[s->top--];
}

void create_prev(struct stack **s)
{
    struct stack *stack; 

    stack = (struct stack *)malloc(sizeof(*stack));
    stack->top = -1;
    memset(stack->data, 0, sizeof(stack->data));

    stack->pop = pop_prev;
    stack->push = push_prev;
    stack->check_top = check_top_prev;
    stack->is_empty = is_empty_prev;
    stack->is_overflow = is_overflow_prev;

    init_stack(stack);

    *s = stack;

}

static void check_top_curr(struct stack *s)
{
    if(s->top <= STACK_BOT_CURR)
        printf("there is no data\n");
    else
        printf("top data is %d\n", s->data[s->top-1]);
}


static void pop_curr(struct stack *s, int val)
{
    if(s->top >= STACK_TOP_CURR){
        printf("overflow!\n");
        return;
    }
        
    s->data[s->top++] = val;
    return;
}

static int push_curr(struct stack *s, int mark)
{
    if(s->top <= STACK_BOT_CURR){
        if(!mark)
            printf("There is no data\n");
        return -1;
    }
    return s->data[--s->top];
}

void create_curr(struct stack **s)
{
    struct stack *stack;

    stack = (struct stack *)malloc(sizeof(*stack));
    stack->top = 0;
    memset(stack->data, 0, sizeof(stack->data));

    stack->pop = pop_curr;
    stack->push = push_curr;
    stack->check_top = check_top_curr;
    stack->is_empty = is_empty_curr;
    stack->is_overflow = is_overflow_curr;

    init_stack(stack);

    *s = stack;
}


void destroy(struct stack **s)
{
    int push;

    printf("push val in stack before it destroy : ");
    while((push = (*s)->push(*s, DESTROY_MARK)) != -1)
    {
        printf("%d ", push);   
    }
    putchar('\n');

}

