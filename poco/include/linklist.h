/* Simple intrusive doubly-linked list helpers for Poco */
#ifndef LINKLIST_H
#define LINKLIST_H

typedef struct Dlnode {
    struct Dlnode *next;
    struct Dlnode *prev;
} Dlnode;

typedef struct Dlheader {
    Dlnode *head;
    Dlnode *tail;
} Dlheader;

#define RNODE_FIELDS Dlnode node; void *resource

static inline void init_list(Dlheader *h)
{
    h->head = 0;
    h->tail = 0;
}

static inline void add_head(Dlheader *h, Dlnode *n)
{
    Dlnode *first = h->head;
    n->prev = 0;
    n->next = first;
    h->head = n;
    if (first) first->prev = n; else h->tail = n;
}

static inline void rem_node(Dlnode *n)
{
    if (n->prev) n->prev->next = n->next;
    if (n->next) n->next->prev = n->prev;
    n->next = n->prev = 0;
}

static inline void rem_from_list(Dlheader *h, Dlnode *n)
{
    if (n->prev) n->prev->next = n->next; else h->head = n->next;
    if (n->next) n->next->prev = n->prev; else h->tail = n->prev;
    n->next = n->prev = 0;
}

#endif /* LINKLIST_H */


