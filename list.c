#include "list.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

list *list_init()
{
	list *l;
    if(!(l = malloc(sizeof *l)))
		return NULL;
	memset(l, 0, sizeof *l);
	return l;
}

int find(list *l, void *val)
{
	//for(int i = 0; i < l->size; i++)
	return 0;
}

bool append(list *l, void *val)
{
	node *tmp;
    if(!(tmp = malloc(sizeof *tmp)))
		return false
	tmp->val = val;
	tmp->next = 0;		
						
	if(!l->back)		
	{					
		l->front = tmp;	
		tmp->prev = 0;
	}
	else
	{
		l->back->next = tmp;
		tmp->next = l->back;
	}
	l->back = tmp;
	l->size++;
	return true;
}


void *pop(list *l)
{
	if(!l->back)
		return 0;
	node *tmp = l->back;
	void *val = tmp->val;
	l->back = tmp->prev;
	if(l->back)
		l->back->next = 0;
	free(tmp);
	l->size--;
	return val;
}

void free_list(list *l)
{
	node *tmp;
	while(l->front)
	{
		tmp = l->front->next;
		free(l->front);
		l->front = tmp;
	}
	free(l);
}

void **list2array(list *l)
{
	void **a;
    if(!(a = (void **)calloc(l->size, sizeof(void*))))
		return 0;
	int count = 0;
	node *tmp = l->front;
	while(tmp)
	{
		a[count++] = tmp->val;
		tmp = tmp->next;
	}
	return a;
}


