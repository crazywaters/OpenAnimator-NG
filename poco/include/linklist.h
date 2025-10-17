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

#define RNODE_FIELDS \
	Dlnode node; \
	void *resource

static inline void init_list(Dlheader *list) {
	static Dlnode sentinel = { &sentinel, &sentinel };
	list->head = list->tail = &sentinel;
}

static inline void add_head(Dlheader *list, Dlnode *node) {
	Dlnode *first = list->head;
	node->prev = NULL;
	node->next = first;
	list->head = node;
	if (first) first->prev = node; else list->tail = node;
}

static inline void rem_node(Dlnode *node) {
	if (node->prev) node->prev->next = node->next; else ;
	if (node->next) node->next->prev = node->prev; else ;
	node->next = node->prev = NULL;
}

#endif /* LINKLIST_H */
#ifndef LINKLIST_H
#define LINKLIST_H

typedef struct Dlnode {
	struct Dlnode *next;
	struct Dlnode *prev;
} Dlnode;

typedef struct Dlheader {
	Dlnode head;
} Dlheader;

#define RNODE_FIELDS Dlnode node; void *resource

static inline void init_list(Dlheader *h)
{
	h->head.next = &h->head;
	h->head.prev = &h->head;
}

static inline void add_head(Dlheader *h, Dlnode *n)
{
	n->next = h->head.next;
	n->prev = &h->head;
	h->head.next->prev = n;
	h->head.next = n;
}

static inline void rem_node(Dlnode *n)
{
	n->prev->next = n->next;
	n->next->prev = n->prev;
	n->next = n->prev = 0;
}

#endif /* LINKLIST_H */


