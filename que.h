#ifndef QUEUE_H
#define QUEUE_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "utils.h"

#define QUE_SIZE 10000

typedef struct
{
	size_t size, head, tail;
	char *val[QUE_SIZE];
} que;

// init
que *que_init(void);
size_t len(que *q);
int is_empty(que *q);
int que_append(que *q, char *val);
char *que_pop(que *q);

// shallow copy 
que *copy(que *q);
que *deepcopy(que *q);

void print_que(que *q);
#endif
