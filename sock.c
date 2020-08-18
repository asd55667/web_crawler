#include "utils.h"
#include "sock.h"
#include "parser.h"

void sock_send(int sock, const char *domain, int port, char *request){
    if(DEBUG) printf("Request info:\n%s\n", request);
    if(sock < 0) error("Fail to opening socket");
    
    struct hostent *host = gethostbyname(domain);
    if(!host) error("No such host");
    
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    memcpy(&serv_addr.sin_addr.s_addr, host->h_addr, host->h_length);
    
    if(connect(sock, (struct sockaddr *)&serv_addr, sizeof (serv_addr)) < 0)
        error("Fail to connect host");
    
    size_t total = strlen(request);
    size_t sent = 0;
    while(sent < total){
//        size_t bytes = write(sock, request + sent, total - sent);
        size_t bytes = send(sock, request + sent, total - sent, 0);
        if(bytes < 0)
            error("Error writing to sock");
        if(bytes == 0)
            break;
        sent += bytes;
    }
}

char *sock_recv(int sock){
    // size should be greater than recv buffer, or may cause multiple call of recv, and the risk of realloc error
    size_t size = 1 << 11;
    char *document = xmalloc(size * sizeof(char));
    
    size_t len = 0;
    char response[BUFSIZ];
    
    if(DEBUG) printf("Response: \n");
    while(1){
        if(len == size - 1){
            size *= 2;
            document = (char *)xrealloc(document, sizeof(char) * size);
        }

        bzero(response, sizeof *response);
        size_t recv_size = recv(sock, response, BUFSIZ, 0);
//        size_t recv_size = read(sock, response, BUFSIZ);
//        printf("%lu\n", recv_size);
        memcpy(document + len, response, recv_size);
//        strcat(document, response);
//        if(DEBUG) fprintf(stdout, "%s\n", response);
        if(recv_size < 0) error("Error reading from sock");
        
        // recv_size == 0 will create an reduntant call of recv, meanwhile the remote has been close already
        if(recv_size == 0){
//        if(recv_size < BUFSIZ){
            close(sock);
            break;
        }
        len += recv_size;
    }
    document[len] = '\0';
    return document;
}
 


