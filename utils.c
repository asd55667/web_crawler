#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void error(const char *s)
{
	perror(s);
//	assert(0);
	exit(EXIT_FAILURE);
}

void file_error(char *s, const char op)
{
	fprintf(stderr, "Failed to %c file: %s\n", op, s);
	exit(EXIT_FAILURE);
}

void alloc_error(const char *s, size_t size)
{
	fprintf(stderr, "fatal: memory exhausted (%s error of %ld bytes).\n", s, size);
	exit(EXIT_FAILURE);
}

void *xmalloc(size_t size)
{
	void *p = malloc(size);
	if(!p)
		alloc_error("Malloc", size);
	return p;
}

void *xcalloc(size_t nitem, size_t size)
{
	void *p = calloc(nitem, size);
	if(!p)
		alloc_error("Calloc", size);
	return p;
}

void *xrealloc(void *p, size_t size)
{
	p = realloc(p, size);
	if(!p)
		alloc_error("Realloc", size);
	return p;
}


