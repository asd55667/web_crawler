#ifndef SOCK
#define SOCK
#include <unistd.h> // read write close
#include <sys/socket.h> // socket connect
#include <netinet/in.h> // sockaddr_in sockaddr
#include <netdb.h> // hostent gethostbyname

#include <openssl/ssl.h>
#include <openssl/err.h>

#include "utils.h"

void sock_send(int sock, const char *domain, int port, char *request);
char *sock_recv(int sock);

char *doc_get(int sock, url_comp *url_component);
#endif
