#ifndef UTILS_H
#define UTILS_H

#ifndef PI
#define PI 3.14159265358979323846
#endif



#include <stdio.h>
#include <time.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>
//#include "que.h"
#include <sys/time.h>

static inline double seconds(){
    struct timeval tp;
    gettimeofday(&tp, NULL);
    return ((double)tp.tv_sec + (double)tp.tv_usec * 1.e-6);
}


// error
void error(const char *s);
void file_error(char *s, const char op);
void alloc_error(const char *s, size_t size);
void *xmalloc(size_t size);
void *xcalloc(size_t nitem, size_t size);
void *xrealloc(void *p, size_t size);

char *fgetls(FILE *fp);
char *fgetl(FILE *fp);


#endif



