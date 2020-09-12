//
//  main.c
//  crawler
//
//  Created by wcw on 2020/6/12.
//  Copyright © 2020年 wcw. All rights reserved.
#include <stdio.h> 
#include <string.h> // memset memcpy
#include <stdlib.h> // free exit
#include <pthread.h>

#include "sock.h"
#include "utils.h"
#include "parser.h"
#include "bloom_filter.h"


typedef struct {
    que *q;
    bloom_filter *bf;
    FILE *fp;
} Args;

#define NTHREADS 6
pthread_t tid[NTHREADS];
pthread_mutex_t lock;

void *thread_func(void *arguments);
int crawler_thread(FILE *fp, bloom_filter *bf, que *q);

int main(int argc, char **argv)
{
    url_comp *url_component;
    url_component = xmalloc(sizeof *url_component);
    bzero(url_component, sizeof(*url_component));
    strcat(url_component->scheme, "http");
    strcat(url_component->host, "news.sohu.com");
    strcat(url_component->path, "/");

    que *q = que_init();
    que_append(q, url_component);
    
    bloom_filter *bf = bf_init(5, 1000000);
        
    double iStart = seconds();
    
    FILE *fp = fopen("b.html", "w");

    Args *args;
    args = xmalloc(sizeof *args);
    args->bf = bf;
    args->q = q;
    args->fp = fp;
    
    if (pthread_mutex_init(&lock, NULL) != 0) {
        printf("\n mutex init has failed\n");
        return 1;
    }
    
    for(int i = 0; i < NTHREADS; i++)
        pthread_create(&(tid[i]), NULL, &thread_func, (void *)args);
    
    for(int i = 0; i < NTHREADS; i++){
        pthread_join(tid[i], NULL);
    }

    pthread_mutex_destroy(&lock);
    free(args);
    free_que(q);
    fclose(fp);
    
    printf("%fs\n", seconds() - iStart);
//    getchar();
    return 0;
}

void *thread_func(void *arguments){
    pthread_mutex_lock(&lock);

    Args *args = (Args *)arguments;
    
    while(args->q->size){
        url_comp *url_ = que_pop(args->q);
        if(!url_) break;
        
        char url[BUFSIZ];
        url_comp_merge(url, url_);

        fprintf(args->fp, "%s\n", url);
        char *document = doc_get(url_);

        doc_parse(document, args->q, args->bf);
    }
    pthread_mutex_unlock(&lock);
    
    return NULL;
}

int crawler_thread(FILE *fp, bloom_filter *bf, que *q){
    Args *args;
    args = xmalloc(sizeof *args);
    args->bf = bf;
    args->q = q;
    args->fp = fp;
    
    if (pthread_mutex_init(&lock, NULL) != 0) {
        printf("\n mutex init has failed\n");
        return 1;
    }
    
    
    for(int i = 0; i < NTHREADS; i++)
        pthread_create(&(tid[i]), NULL, &thread_func, (void *)args);
    
    for(int i = 0; i < NTHREADS; i++){
        pthread_join(tid[i], NULL);
    }
    pthread_mutex_destroy(&lock);
    free(args);
    return 0;
}
