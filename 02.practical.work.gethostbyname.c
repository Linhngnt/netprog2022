#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <netdb.h>

int DNSlookup(char * hostname , char* ip);
 int main(int argc , char *argv[]){
        if(argc < 2) // this function has 2 arguments (hostname & ip)//
        {
            printf("Cannot do it!");
            exit(1);
        }
char *hostname = argv[1];  //name of host//    
char ip[100];     //the correspond ip//
DNSlookup(hostname,ip);     //called the function//
printf("%s have IP is: %s\n", hostname, ip);     
}

int DNSlookup(char * hostname , char* ip){
        struct hostent *host_name;
        struct in_addr **addr_list; //list of address from name server//
        int i;
        if ( (host_name = gethostbyname(hostname) ) == NULL) {
                herror("gethostbyname error");             
                return 1;     
        }     
        addr_list = (struct in_addr **) host_name->h_addr_list;     
        for(i = 0; addr_list[i] != NULL; i++) {   //look up for the correspond ip address against the provided hostname             
                strcpy(ip , inet_ntoa(*addr_list[i]));             
                return 0;}     //the correspond IP is found//   
                return 1;  //the correspond IP is not found//
}