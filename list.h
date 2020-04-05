#ifndef LIST_H
#define LIST_H

typedef struct
{
	void *val;
	node *prev, *next;
} node;

typedef struct
{
	int size;
	node *front, *back;
} list;

// init
list *list_init();

// find index of val
int find(list *l, void *val);

bool append(list *l, void *val);

void *pop(list *l);

void free_list(list *l);

void **list2array(list *l);

#endif
