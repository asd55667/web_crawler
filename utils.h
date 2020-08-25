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

#include <sys/time.h>


typedef struct {
    char scheme[1<<4];
    char host[1<<6];
    char path[BUFSIZ];
    char query[BUFSIZ];
    char fragment[1<<6];
} url_comp;

static inline void url_comp_merge(char *url, url_comp *url_components){
    bzero(url, sizeof(url));
    strcat(url, url_components->scheme);
    strcat(url, "://");
    strcat(url, url_components->host);
    strcat(url, url_components->path);
    strcat(url, url_components->query);
    strcat(url, url_components->fragment);
}

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



