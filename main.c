#include <stdio.h> // printf
#include <string.h> // memset memcpy
#include <stdlib.h> // free exit
#include <sys/unistd.h> // read write close

#include <sys/socket.h> // socket connect
#include <netinet/in.h> // sockaddr_in sockaddr
#include <netdb.h> // hostent gethostbyname

#include "utils.h"
#include "bloom_filter.h"

void error(const char * msg) {perror(msg); exit(0);}

int main(int argc, char ** argv)
{
    // program.exe --domain --port
    
//    const char *domain = strlen(argv[1]) > 0 ? argv[1] : "localhost";
//    const int port = atoi(argv[2]) > 0 ? atoi(argv[2]) : 80;
    struct hostent *
    
    
    
    

    
//    printf("%d", sizeof);
    
    getchar();
    return 0;
}
