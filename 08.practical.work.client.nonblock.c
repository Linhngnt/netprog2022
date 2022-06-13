#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h> 
#include<netdb.h>
#include <fcntl.h>
#include <poll.h> 
#include<unistd.h>

int main(){
    setbuf(stdout, NULL);
    char hostname[1234];
    struct sockaddr_in saddr;
    struct hostent *h;
    int sockfd;
    unsigned short port = 8784;
    char clientms[1234], serverms[1234], ms[1234];
    
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

    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int));
    
    int sockfl = fcntl(sockfd, F_GETFL, 0);
    sockfl |= O_NONBLOCK;
    fcntl(sockfd, F_SETFL, sockfl);

    struct pollfd stdin_pollfd;
    stdin_pollfd.fd = fileno(stdin);
    stdin_pollfd.events = POLLIN;

    if (connect(sockfd, (struct sockaddr *) &saddr, sizeof(saddr)) < 0) {
        printf("Cannot connect\n");
        return 1;
    }
    printf("Connected\n");

    while (1) {
        printf("Client: ");
        fflush(stdout);
        if(recv(sockfd, serverms, sizeof(serverms), 0)>0) {
            int i=0;
            do {
                ms[i]= serverms[i];
            } 
            while (serverms[i++] != '\0');
            printf("\r Server: ");
            printf("%s", ms); 
        } 
        while (clientms[strlen(clientms) - 1] != '\n');
        printf("Message from server: ");

        if (poll(&stdin_pollfd,1,0) > 0){
            if (stdin_pollfd.revents &POLLIN){
                fgets(clientms,sizeof(clientms),stdin); 
                if ((strncmp(clientms, "/quit\n", 6)) == 0) {
                    printf("Server disconnected.\n");
                    return 0;
                }
                send(sockfd , clientms, strlen(clientms) + 1, 0);  
            }
        }
    }
    close(sockfd);
    return 0;
}