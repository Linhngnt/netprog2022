#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

int main(int argc, char *argv[]) {
    struct sockaddr_in saddr;
    struct hostent *h;
    struct in_addr **addr_list;
    int sockfd;
    unsigned short port = 8784; 
    
    if ((sockfd=socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Error creating socket\n");
        return 1;
    }

    if(argc < 2) {
            printf("Cannot do it!");
            exit(1);
        }
    
    char *hostname = argv[1];    
    char ip[100];      

    h = gethostbyname(hostname);

    if (h == NULL) {
        printf("Unknown host\n");
        return 1;
    }
    
    int i = 0;
    addr_list = (struct in_addr **) h->h_addr_list;     
        for(i = 0; addr_list[i] != NULL; i++) {              
            strcpy(ip , inet_ntoa(*addr_list[i]));             
            printf("%s have IP is: %s\n", hostname, ip);
            return 0;
            }  
            printf("Cannot find");
            return 1;  

    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    memcpy((char *) &saddr.sin_addr.s_addr, h->h_addr_list[0], h->h_length);
    saddr.sin_port = htons(port);

    if (connect(sockfd, (struct sockaddr *) &saddr, sizeof(saddr)) < 0) {
        printf("Cannot connect\n");
        return 1;
    }
    printf("Connected\n");
    return 0;
}