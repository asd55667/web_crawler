#ifndef SOCK
#define SOCK
#include <unistd.h> // read write close
#include <sys/socket.h> // socket connect
#include <netinet/in.h> // sockaddr_in sockaddr
#include <netdb.h> // hostent gethostbyname
#include <arpa/inet.h>

#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#include "utils.h"

void sock_send(int sock, char *request);
char *sock_recv(int sock);

void ssl_send(BIO* bio, char* request);
char *ssl_recv(BIO *bio);

char *doc_get(url_comp *url_component);
#endif
