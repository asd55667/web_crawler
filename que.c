#include "que.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

que *que_init()
{
	que *q;
    q = xmalloc(sizeof *q);
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

int que_append(que *q, url_comp *val)
{
    if(full(q)) return 0;
	q->val[q->tail] = val;
	q->tail = (q->tail + 1) % QUE_SIZE;
	q->size++;
    return 1;
}

int full(que *q){
    if((q->head - q->tail + QUE_SIZE) % QUE_SIZE == 1)
        return 1;
    return 0;
}

url_comp *que_pop(que *q)
{
	if(is_empty(q))
		return NULL;
	size_t head = q->head;
	q->head = (q->head + 1) % QUE_SIZE;
	q->size--;
    
//    url_comp *front;
//    front = xmalloc(sizeof *front);
//    bzero(front, sizeof *front);
    
    url_comp *tmp = q->val[head];
    q->val[head] = NULL;
    
    return tmp;
    
    return q->val[head];
}

void free_que(que *q){
    for(int i = 0; i < q->size; i++)
        free(q->val[i]);
    free(q);
}

que *copy(que *q)
{
	que *newq;
    newq = xmalloc(sizeof *q);
	memcpy(newq, q, sizeof *q);
	return newq;
}

//que *deepcopy(que *q)
//{
//	que *newq;
//    newq = xmalloc(sizeof *q);
//	newq->size = q->size;
//	newq->head = q->head;
//	newq->tail = q->tail;
//	for(int i = 0; i < q->size; i++)
//		newq->val[i] = strdup(q->val[i]);
//	return newq;
//}
//
//
//void print_que(que *q)
//{
//	if(q)
//	{
//        for(int i = 0; i < q->size; i++)
//            printf("%d: %s\n", i, q->val[i]);
////		printf("size: %ld", q->size);
//	}
//}
//
//
//int test_que()
//{
//    // que test
//    que *q = que_init();
//    que_append(q, "ASDSAD");
//    que_append(q, "QWE");
//    print_que(q);
//
//    if(!is_empty(q))
//        printf("len: %ld\n", len(q));
//    printf("pop: %s\n", que_pop(q));
//    printf("len: %ld\n", len(q));
//
//    que *qq = copy(q);
//    que *qqq = deepcopy(q);
//    free(q);
//    free(qq);
//    free(qqq);
//
//    getchar();
//    return 0;
//
//}
