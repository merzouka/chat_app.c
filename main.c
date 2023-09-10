#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

void printAddressType(int socket){
    struct sockaddr_storage addr;
    socklen_t len = sizeof(struct sockaddr_storage);
    getsockname(socket, (struct sockaddr *)(&addr), &len);
    switch (addr.ss_family) {
        case AF_INET:
            printf("IPv4 protocol.\n");
            break;
        case AF_INET6:
            printf("IPv6 protocol.\n");
            break;
    }
}

void printAddr(struct sockaddr *address){
    char *s = malloc(INET6_ADDRSTRLEN);
    char *tmp;
    switch (address->sa_family) {
        case AF_INET:
            tmp = (char *)inet_ntop(AF_INET, &((struct sockaddr_in *)address)->sin_addr.s_addr, s, INET_ADDRSTRLEN);
            break;
        case AF_INET6:
            tmp = (char *)inet_ntop(AF_INET, &((struct sockaddr_in6 *)address)->sin6_addr, s, INET6_ADDRSTRLEN);
            break;
    }
    if (tmp == NULL){
        printf("failed to get address.\n");
        exit(1);
    }
    printf("address: %s\n", s);
    free(s);
}

void printAddressInfo(struct sockaddr *address){
    struct sockaddr_in addr4;
    struct sockaddr_in6 addr6;
    switch (address->sa_family) {
        case AF_INET:
            addr4 = *((struct sockaddr_in *)address);
            printf("family: IPv4\n");
            printAddr(address);
            printf("port: %d\n", addr4.sin_port);
            break;
        case AF_INET6:
            addr6 = *((struct sockaddr_in6 *)address);
            printf("family: IPv6\n");
            printAddr(address);
            printf("port: %d\n", addr6.sin6_port);
            break;
    }
}

int main(void){
    int sock = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);
    struct sockaddr_in6 address = {.sin6_family = AF_INET6, .sin6_addr = in6addr_any, .sin6_port = 0};
    if (bind(sock, (struct sockaddr *)(&address), sizeof(struct sockaddr_in6)) != 0){
        printf("couldn't bind to address:\n");
        printAddressInfo((struct sockaddr *)(&address));
        exit(1);
    }
    printAddressInfo((struct sockaddr *)(&address));
    close(sock);

    return 0;
} 
