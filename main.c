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




int main(int argc, char **argv)
{
    
    url_comp *url_component;
    url_component = xmalloc(sizeof *url_component);
    bzero(url_component, sizeof(*url_component));
    strcat(url_component->host, "news.sohu.com");
    strcat(url_component->path, "/");
    que *q = que_init();
    que_append(q, url_component);
    
    double iStart = seconds();
    
    int cnt = 0;
    FILE *fp = fopen("a.html", "w");
    
    while (cnt < 10000){
        int sock = socket(AF_INET, SOCK_STREAM, 0);
        url_comp *url_ = que_pop(q);
        char url[BUFSIZ];
        bzero(url, BUFSIZ);
        strcat(url, url_->scheme);
        strcat(url, "://");
        strcat(url, url_->host);
        strcat(url, url_->path);
        strcat(url, url_->query);
        strcat(url, url_->fragment);
        fprintf(fp, "%s\n", url);
        char *document = doc_get(sock, url_);
        doc_parse(document, q);
//        free(document);
    }
    free_que(q);
    printf("%fs\n", seconds() - iStart);
    
//    FILE *fp = fopen("a.html", "w");
//    printf("%lu\n", strlen(document));
//    fprintf(fp, "%s", document);
//    FILE *fp = fopen("a.html", "r");
//    char *document = fgetls(fp);
    fclose(fp);
    
//    free(document);
    

//    getchar();
    return 0;
}
