#ifndef QUEUE_H
#define QUEUE_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_Q 10000

typedef struct
{
	size_t size, head, tail;
	char *val[MAX_Q];
} que;

// init
que *que_init();
size_t len(que *q);
int is_empty(que *q);
int append(que *q, char *val);
char *pop(que *q);

// shallow copy 
que *copy(que *q);
que *deepcopy(que *q);

void print_que(que *q);
#endif
