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
#include "parser.h"
#include "bloom_filter.h"


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
    int cnt = 0;
    while (cnt < 1000){
        url_comp *url_ = que_pop(q);
        if(!url_) break;

        char url[BUFSIZ];
        url_comp_merge(url, url_);

        fprintf(fp, "%s\n", url);
        char *document = doc_get(url_);

        doc_parse(document, q, bf);
        cnt++;
    }
    
//    FILE *fp = fopen("a.html", "r");
//    char *document = fgetls(fp);
//    doc_parse(document, q, bf);
    

    free_que(q);
    fclose(fp);
    printf("%fs\n", seconds() - iStart);

//    getchar();
    return 0;
}
