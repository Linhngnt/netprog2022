#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h> 
#include<netdb.h>
#include <fcntl.h>
#include <poll.h>
#include <unistd.h>

int main(){
    setbuf(stdout, NULL);
    int sockfd, clen, clientfd;
    struct sockaddr_in saddr, caddr;
    unsigned short port = 8784;
    char clientms[1234], serverms[1234], ms[1234];

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        printf("Error creating socket\n");
        return 1;
    }

    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int));

    int sockfl = fcntl(sockfd, F_GETFL, 0);
    sockfl |= O_NONBLOCK;
    fcntl(sockfd, F_SETFL, sockfl);


    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);
    saddr.sin_port = htons(port);

    if (bind(sockfd, (struct sockaddr *)&saddr, sizeof(saddr)) < 0){
        perror("Error binding");
        return 1;
    }

    if (listen(sockfd, 5) < 0){
        perror("Error listening");
        return 1;
    }   
    
    struct pollfd stdin_pollfd;
    stdin_pollfd.fd = fileno(stdin);
    stdin_pollfd.events = POLLIN;

    clen = sizeof(caddr);
    if ((clientfd = accept(sockfd, (struct sockaddr *)&caddr, &clen)) < 0){
        perror("Error accepting connection");
        return 1;
    }

    printf("Connected!\n");

    while (1) {
        printf("Message from client: ");
        fflush(stdout);
        if(recv(clientfd, clientms, sizeof(clientms),0)> 0) { 
            int i=0;
            do {
                ms[i]= clientms[i]; 
            } 
            while(ms[i++] != '\0');
                printf("\nClient: ");
                printf("%s", ms); 
                } 

        if (poll(&stdin_pollfd,1,0) > 0){
            if (stdin_pollfd.revents &POLLIN){
                fgets(serverms, sizeof(serverms),stdin);  
                if (strncmp("/dc\n", serverms, 4) == 0) {
                    printf("Client disconnected.\n");
                    return 0;
                }
                send(clientfd, serverms, strlen(serverms) + 1, 0);
                }
        }
        close(clientfd);
    }
    close(sockfd);
    return 0;
}