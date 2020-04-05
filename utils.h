#ifndef UTILS_H
#define UTILS_H

#ifndef PI
#define PI 3.14159265358979323846
#endif

#include <stdio.h>
#include <time.h>
#include "que.h"

// error
void error(const char *s);
void file_error(char *s, const char op);
void alloc_error(const char *s, size_t size);
void *xmalloc(size_t size);
void *xcalloc(size_t nitem, size_t size);
void *xrealloc(void *p, size_t size);


#endif



