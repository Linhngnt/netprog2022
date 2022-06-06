#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h> 
#include<netdb.h>

int main(){
    char hostname[1234];
    struct sockaddr_in saddr;
    struct hostent *h;
    int sockfd;
    unsigned short port = 8784;
    char clientms[1234], serverms[1234];
    
    if ((sockfd=socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Error creating socket \n");
        return 1;
    }
    
    struct in_addr *address;
    printf("Enter host domain name: ");
    fgets(hostname, 1234, stdin);
    hostname[strlen(hostname) - 1] = '\0';

    if ((h=gethostbyname(hostname)) == NULL) {
        printf("Unknown host \n");
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

    while (1){
        printf("Client: ");
        do
        {
            fgets(clientms, 1234, stdin);
            if (send(sockfd, clientms, strlen(clientms) + 1, 0)<0){
                printf("Can not send to server!!!");
            };
        } 
        while (clientms[strlen(clientms) - 1] != '\n');
        printf("Message from server: ");
        do
        {
            if (recv(sockfd, serverms, 1234, 0) <0) {
                printf("Can not get server's message!!!");
            }
            printf("%s\n",serverms);
        } while (serverms[strlen(serverms) - 1] != '\n');
    }
    return 0;
}