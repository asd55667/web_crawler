#ifndef SOCK
#define SOCK
#include <unistd.h> // read write close
#include <sys/socket.h> // socket connect
#include <netinet/in.h> // sockaddr_in sockaddr
#include <netdb.h> // hostent gethostbyname


void sock_send(int sock, const char *domain, int port, char *request);
char *sock_recv(int sock);


#endif
