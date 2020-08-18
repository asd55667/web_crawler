#ifndef LIST_H
#define LIST_H

#include "utils.h"

typedef struct node_tmp{
	void *val;
	struct node_tmp *prev, *next;
} node;

typedef struct {
	int size;
	node *front, *back;
} list;

// init
list *list_init(void);

// find index of val
int find(list *l, void *val);

int list_append(list *l, void *val);

void *list_pop(list *l);

void free_list(list *l);

void **list2array(list *l);

#endif
