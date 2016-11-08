#ifndef _DOUBLE_LIST_H
#define _DOUBLE_LIST_H


#define each_double_circle_list(head, pos)\
        for(pos = head->next; pos != head; pos = pos->next)

#define each_double_circle_list_prev(head, pos)\
        for(pos = head->prev; pos != head; pos = pos->prev)

struct data{
    int value;
};

struct double_list{
    
    struct data data;
        
    struct double_list *next;
    struct double_list *prev;
};

extern void create_circle(struct double_list **);
extern void insert_prev_node(struct double_list *, struct double_list *);
extern void insert_tail_node(struct double_list *, struct double_list *);
extern int select_node(struct double_list *, int );
extern void delete_node(struct double_list *);
extern void destroy_list(struct double_list *);

#endif
