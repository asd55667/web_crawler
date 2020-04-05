#include "que.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

que *que_init()
{
	que *q;
	if(!(q = malloc(sizeof *q)))
		return NULL;
	memset(q, 0, sizeof *q);
	return q;
}

size_t len(que *q)
{
	size_t length = q->tail - q->head;
	if(q->size != length)
		printf("size dismatch! %ld: %ld\n", q->size, length);
	return length;

}

int is_empty(que *q)
{
	if(q->head == q->tail)
		return 1;
	return 0;
}

int append(que *q, char *val)
{
	if((q->head - q->tail + MAX_Q) % MAX_Q == 1)
        return 0;
	q->val[q->tail] = val;
	q->tail = (q->tail + 1) % MAX_Q;
	q->size++;
    return 1;
}

char *pop(que *q)
{
	if(is_empty(q))
		return NULL;
	size_t head = q->head;
	q->head = (q->head + 1) % MAX_Q;
	q->size--;
	return q->val[head];
}

que *copy(que *q)
{
	que *newq;
    if(!(newq = malloc(sizeof *q)))
        return NULL;
	memcpy(newq, q, sizeof *q);
	return newq;
}

que *deepcopy(que *q)
{	
	que *newq;
	if(!(newq = malloc(sizeof *q)))
		return NULL;
	newq->size = q->size;
	newq->head = q->head;
	newq->tail = q->tail;
	for(int i = 0; i < q->size; i++)
		newq->val[i] = strdup(q->val[i]);
	return newq;
}


void print_que(que *q)
{
	if(q)
	{
        for(int i = 0; i < q->size; i++)
            printf("%d: %s\n", i, q->val[i]);
//		printf("size: %ld", q->size);
	}
}	


int test_que()
{
    // que test
    que *q = que_init();
    append(q, "ASDSAD");
    append(q, "QWE");
    print_que(q);
    
    if(!is_empty(q))
        printf("len: %ld\n", len(q));
    printf("pop: %s\n", pop(q));
    printf("len: %ld\n", len(q));
    
    que *qq = copy(q);
    que *qqq = deepcopy(q);
    free(q);
    free(qq);
    free(qqq);
    
    getchar();
    return 0;
    
}
