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
    struct sockaddr_in client_addr;
    char buffer[1024] = { 0 };
    char *str_addr[2];

    if (argc != 2)
    {
        printf("argv[1] == \"xxxxxx\" bindport.\n");
        return EXIT_SUCCESS;
    }

    errno = 0;

    int sock = socket(AF_INET, SOCK_DGRAM, 0); 
    error(sock, "socket()");

    make_addr(&addr, NULL, (unsigned short)atoi(argv[1]));

    bind(sock, (struct sockaddr*)&addr, sizeof(addr));
    error(sock, "bind()");

    while(1)
    {   
        memset(buffer, 0, sizeof(char)*1024);

        ssize_t bytes = recvfrom(sock, (char*)&buffer, 1024, 0, NULL, NULL);
        error(sock, "recvfrom()");

        unsigned int peerip;
        unsigned short peerport;
        memcpy((char*)&peerip, buffer, sizeof(unsigned));
        memcpy((char*)&peerport, buffer + sizeof(unsigned), sizeof(unsigned short));

        //Swap endianess
        peerport = (peerport>>8) | (peerport<<8);
        peerip =  ((peerip >> 24) & 0x000000ff) | 
                  ((peerip << 8 ) & 0x00ff0000) | 
                  ((peerip >> 8 ) & 0x0000ff00) | 
                  ((peerip << 24) & 0xff000000); 
        
        
        make_addr(&client_addr, NULL, peerport);
        client_addr.sin_addr.s_addr = htonl(peerip);
        
        sendto(sock, buffer + 6, strlen(buffer + 6) + 1, 0, (struct sockaddr*)&client_addr, sizeof(client_addr));
        error(sock, "sendto()");

        if (strcmp(buffer+6, "stop") == 0) break;
    }

    close(sock);
    return 0;
}
