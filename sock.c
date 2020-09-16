#include "sock.h"

void build_connection(int sock, const char *domain, int port){
    
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
}

void init_openssl()
{
    SSL_load_error_strings();
    SSL_library_init();
    ERR_load_BIO_strings();
    OpenSSL_add_ssl_algorithms();
}



SSL_CTX *create_context()
{
    const SSL_METHOD *method;
    SSL_CTX *ctx = NULL;

    method = SSLv23_client_method();

    ctx = SSL_CTX_new(method);
    if (!ctx) {
    perror("Unable to create SSL context");
    ERR_print_errors_fp(stderr);
    exit(EXIT_FAILURE);
    }
    int r = SSL_CTX_load_verify_locations(ctx, "cert.pem", NULL);
    if(!r){
        perror("Unable to load the trust cert from cert.pem\n");
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }
    return ctx;
}

BIO *create_bio(SSL_CTX *ctx, SSL *ssl, char *host, int port){
    BIO* bio = NULL;
    bio = BIO_new_ssl_connect(ctx);
    BIO_get_ssl(bio, &ssl);
    if(!ssl){
        perror("Unable to allocate SSL pointer.\n");
        exit(EXIT_FAILURE);
    }
    SSL_set_mode(ssl, SSL_MODE_AUTO_RETRY);
    
    char host_and_port[BUFSIZ];
    sprintf(host_and_port, "%s:%d", host, port);
    
    BIO_set_conn_hostname(bio, host_and_port);
    if (BIO_do_connect(bio) < 1) {
        perror("Unable to connect BIO.%s\n");
        exit(EXIT_FAILURE);
    }
//    if (SSL_get_verify_result(ssl) != X509_V_OK) {
//        perror("Unable to verify connection result.\n");
//        exit(EXIT_FAILURE);
//    }
    return bio;
}

void ssl_send(BIO* bio, char* request){
    size_t total = strlen(request);
    size_t sent = 0;
    while(sent < total){
        int bytes = BIO_write(bio, request + sent, (int)(total - sent));
        if(bytes < 0)
            error("Error writing to sock");
        if(bytes == 0)
            break;
        sent += bytes;
    }
}

char *ssl_recv(BIO *bio){
    // size should be greater than recv buffer, or may cause multiple call of recv, and the risk of realloc error
    size_t size = 1 << 30;
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
//        size_t recv_size = recv(sock, response, BUFSIZ, 0);
        size_t recv_size =  BIO_read(bio, response, BUFSIZ);
//        printf("%lu\n", recv_size);
//        memcpy(document + len, response, recv_size);
        strcat(document, response);
        if(DEBUG && len < 1024) fprintf(stdout, "%s\n", response);
        if(recv_size < 0) error("Error reading from sock");
        
        if(recv_size == 0){
//        if(recv_size < BUFSIZ){
            break;
        }
        len += recv_size;
    }
    document[len] = '\0';
    return document;
}
    
void cleanup_openssl()
{
    EVP_cleanup();
}

void sock_send(int sock, char *request){
    if(DEBUG) printf("Request info:\n%s\n", request);
    size_t total = strlen(request);
    size_t sent = 0;
    while(sent < total){
        int bytes = (int)write(sock, request + sent, (int)(total - sent));
        if(bytes < 0)
            error("Error writing to sock");
        if(bytes == 0)
            break;
        sent += bytes;
    }
}

char *sock_recv(int sock){
    // size should be greater than recv buffer, or may cause multiple call of recv, and the risk of realloc error
    size_t size = 1 << 30;
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
        size_t recv_size =  read(sock, response, BUFSIZ);
//        printf("%lu\n", recv_size);
//        memcpy(document + len, response, recv_size);
        strcat(document, response);
        if(DEBUG && len < 1024) fprintf(stdout, "%s\n", response);
        if(recv_size < 0) error("Error reading from sock");
        
        if(recv_size == 0){
            break;
        }
        len += recv_size;
    }
    document[len] = '\0';
    return document;
}
 
void _send(void *sockfd, char *request){
    if(DEBUG) printf("Request info:\n%s\n", request);
    size_t total = strlen(request);
    size_t sent = 0;
        
    while(sent < total){
        int bytes;
        if(sizeof sockfd == 4) {
            sockfd = (int *)sockfd;
            bytes = send(sockfd, request + sent, (int)(total - sent), 0);
        }
        else{
            sockfd = (SSL *)sockfd;
            bytes = SSL_write(sockfd, request + sent, (int)(total - sent));
        }
        
        if(bytes < 0)
            error("Error writing to sock");
        if(bytes == 0)
            break;
        sent += bytes;
    }
}

char *_recv(size_t (*callback)(void *, char *, int), void *sockfd){
    // size should be greater than recv buffer, or may cause multiple call of recv, and the risk of realloc error
    size_t size = 1 << 30;
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
        size_t recv_size =  callback(sockfd, response, BUFSIZ);
//        printf("%lu\n", recv_size);
//        memcpy(document + len, response, recv_size);
        strcat(document, response);
        if(DEBUG && len < 1024) fprintf(stdout, "%s\n", response);
        if(recv_size < 0) error("Error reading from sock");
        
        if(recv_size == 0){
            break;
        }
        len += recv_size;
    }
    document[len] = '\0';
    return document;
}


char *doc_get(url_comp *url_component){
    char request[BUFSIZ];
    char path[BUFSIZ];
    bzero(path, sizeof(char) * BUFSIZ);
    bzero(request, sizeof(char) * BUFSIZ);
    if(!*url_component->path) strcat(path, "/");
    else strcat(path, url_component->path);
    strcat(path, url_component->query);
    
    sprintf(request, "GET %s HTTP/1.1\r\n"
                    "Host: %s\r\n"
                    "User-Agent: wcw-c-demo\r\n"
                    "Connection: Close\r\n"
            "\r\n",  path, url_component->host);
    
    
    int port;
    char *document = "";
    if(strcmp(url_component->scheme, "https")){
        int sock = socket(AF_INET, SOCK_STREAM, 0);
        port = 80;
        build_connection(sock, url_component->host, port);
        
        sock_send(sock, request);
//        _send((void *)&sock, request);
        document = sock_recv(sock);
        close(sock);
    }
    else{
        port = 443;
        
        init_openssl();
        SSL_CTX *ctx = create_context();
        SSL *ssl = NULL;
        BIO *bio = create_bio(ctx, ssl, url_component->host, port);
        
        ssl_send(bio, request);
        document = ssl_recv(bio);
        
        SSL_free(ssl);
        SSL_CTX_free(ctx);
        cleanup_openssl();
    }
    free(url_component);

    return document;
}

