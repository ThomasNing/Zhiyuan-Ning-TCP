/*
** client.c -- a stream socket client demo
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>


#define PORT "21153" // the port client will be connecting to 

#define MAXDATASIZE 100 // max number of bytes we can get at once 
#define Maxlen 256


// get sockaddr, IPv4 or IPv6:
void transfer(int sockfd, char* name1){  
    
    int charsent = send(sockfd, name1, sizeof(name1), 0); 
    printf("The client sent greetings to the server\n");

    
    (sockfd, name1, sizeof(name1), 0);
    
    printf( "Get reply from ");
    printf(name1);
    printf("\n");
}

void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(int argc, char *argv[])
{
    printf("Client Terminal\n");
    int sockfd, numbytes;  
    char buf[MAXDATASIZE];
    struct addrinfo hints, *servinfo, *p;
    int rv;
    char s[INET6_ADDRSTRLEN];

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo("localhost", PORT, &hints, &servinfo)) != 0) { //get_in_addr(servinfo)->sin_port 
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }    

    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("client: socket");
            continue;
        }

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("client: connect");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "client: failed to connect\n");
        return 2;
    }

    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
            s, sizeof s);
    printf("The client is up and running \n");
    
    transfer(sockfd, argv[1]);

    freeaddrinfo(servinfo); // all done with this structure

    if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
        //perror("recv");
        exit(1);
    }

    buf[numbytes] = '\0';

    close(sockfd);

    return 0;
}