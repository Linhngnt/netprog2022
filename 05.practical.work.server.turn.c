#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h> 
#include<netdb.h>

int main() {
    int sockfd, clen, clientfd;
    struct sockaddr_in saddr, caddr;
    unsigned short port = 8784; 
    char clientms[1000], serverms[1000];

	if ((sockfd=socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Error creating socket\n");
        return 1;
    }

    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);
    saddr.sin_port = htons(port);
    
    if (bind(sockfd, (struct sockaddr *)&saddr, sizeof(saddr)) < 0) {
        perror("Error binding");
        return 1;
    }

    if (listen(sockfd, 5) < 0) {
        perror("Error listening");
        return 1;
    }

    clen = sizeof(caddr);
    if ((clientfd = accept(sockfd, (struct sockaddr *)&caddr, &clen)) < 0) {
        perror("Error accepting connection");
        return 1;
    }
    printf("Connected\n");

    while (1) {
        if (recv(clientfd, clientms, sizeof(clientms), 0) < 0) {
            printf("Cannot get message from client");
        }
        printf("Client: %s \n", clientms);

        printf("Message to client: ");
        scanf("%s", serverms);

        if (send(clientfd, serverms, strlen(serverms) + 1, 0) < 0) {
            printf("Cannot send to client\n");
        }
    }
    return 0;
}
	



