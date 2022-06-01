#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h> 
#include<netdb.h>

int main() {
    struct sockaddr_in saddr;
    struct hostent *h;
    int sockfd;
    unsigned short port = 8784; 
    char clientms[1000], serverms[1000];
    
    if ((sockfd=socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Error creating socket\n");
        return 1;
    }

    char hostname[100];    
    struct in_addr *address;
    printf("Enter host domain name: ");
    scanf("%s", hostname); 

    if ((h=gethostbyname(hostname)) == NULL) {
        printf("Unknown host\n");
        return 1;
    }
    
    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    memcpy((char *) &saddr.sin_addr.s_addr, h->h_addr_list[0], h->h_length);
    saddr.sin_port = htons(port);

    if (connect(sockfd, (struct sockaddr *) &saddr, sizeof(saddr)) < 0) {
        printf("Cannot connect\n");
        return 1;
    }
    printf("Connected\n");
    
    while (1) {
        printf("Message to server: ");
        scanf("%s", clientms);

        if (send(sockfd, clientms, strlen(clientms) + 1, 0) < 0) {
            printf("Cannot send to server");
        }
        if (recv(sockfd, serverms, sizeof(serverms), 0) < 0) {
            printf("Cannot get message from server");
        }
    }
    return 0;
}


