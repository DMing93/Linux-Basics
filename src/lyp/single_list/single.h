#ifndef _SINGLE_H
#define _SINGLE_H

struct date {
    int no;
};

struct single_list {
    struct date date;
    struct single_list *next;
};

#define each_list(head, pos) \
        for(pos = head->next; pos; pos = tmp->next)


#define each_list_safe_del(head, pos, prev) \
        for(pos = head->next, prev = head; pos; prev = pos, pos = pos->next)
        
//extern struct single_list *create(struct single_list *);
extern void create(struct single_list **);

extern void insert_node(struct single_list *, struct single_list *);

extern struct single_list *select_node(int value, struct single_list *);

extern struct single_list *delete_node(int value, struct single_list *head);
extern void delete_all_node(struct single_list *head);

#endif
