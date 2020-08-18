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
    //const char *domain = strlen(argv[1]) > 0 ? argv[1] : "localhost";
    //const int port = atoi(argv[2]) > 0 ? atoi(argv[2]) : 80;
    //    const char *domain = "mini.wuchengwei.icu";
    
  	const char *domain = "news.sohu.com";
   	const int port = 80;

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    
    char request[BUFSIZ];
    // http/1.1 must have host field
    char *head = "GET / HTTP/1.1\r\n";
    sprintf(request, "%sHost: %s\r\n", head, domain);

    strcat(request, "User-Agent: wcw-c-demo\r\n");
    strcat(request, "Connection: Close\r\n");
    strcat(request, "\r\n");
    
    sock_send(sock, domain, port, request);

    double iStart = seconds();
    
    char *document = sock_recv(sock);
    
    double iEnd = seconds();
    printf("\n%fs\n", iEnd - iStart);
    
    doc_parse(document);
    printf("%fs\n", seconds() - iEnd);
    
//    FILE *fp = fopen("a.html", "w");
//    printf("%lu\n", strlen(document));
//    fprintf(fp, "%s", document);
//    FILE *fp = fopen("a.html", "r");
//    char *document = fgetls(fp);
//    fclose(fp);
    
    free(document);
    

//    getchar();
    return 0;
}
