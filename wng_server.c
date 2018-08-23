#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <netdb.h>
#include <time.h>

void error(int s, const char *message)
{
    if (errno == 0) return;

    close(s);
    perror(message);
    exit(1);
}

void make_addr(struct sockaddr_in *addr, char *ip, unsigned short port)
{
    memset(addr->sin_zero, 0, sizeof(char)*8);
    addr->sin_port = htons(port);
    addr->sin_family = AF_INET;

    if (ip == NULL) 
	    addr->sin_addr.s_addr = htonl(INADDR_ANY);
    else
	    inet_aton(ip, &addr->sin_addr);
}

void split_addr(char *string, const char *symb, char *addr[2])
{
    addr[0] = strtok(string, symb);
    addr[1] = strtok(NULL, symb);
}

int main(int argc, char *argv[])
{
    socklen_t s_recvaddr;
    struct sockaddr_in addr;
    struct sockaddr_in next_router_addr;
    char buffer[1024] = { 0 };
    char *str_addr[2];

    if (argc != 3)
    {
        printf("argv[1] == \"xxxxxx\" bindport.\n");
        printf("argv[2] == \"xxx.xxx.xxx.xxx:xxxxx\" adress of next router in chain.\n");
        return EXIT_SUCCESS;
    }

    errno = 0;

    split_addr(argv[2], ":", str_addr);

    int sock = socket(AF_INET, SOCK_DGRAM, 0); 
    error(sock, "socket()");

    make_addr(&addr, NULL, (unsigned short)atoi(argv[1]));
    make_addr(&next_router_addr, str_addr[0], (unsigned short)atoi(str_addr[1]));

    bind(sock, (struct sockaddr*)&addr, sizeof(addr));
    error(sock, "bind()");

    while(1)
    {   
        memset(buffer, 0, sizeof(char)*1024);

        recvfrom(sock, (char*)&buffer, 1024, 0, NULL, NULL);
        error(sock, "recvfrom()");
    
        sendto(sock, buffer , strlen(buffer) + 1, 0, 
               (struct sockaddr*)&next_router_addr, sizeof(next_router_addr));
        error(sock, "sendto()");

        if (strcmp(buffer+6, "stop") == 0) break;
    }

    close(sock);
    return 0;
}
