#include "csapp.h"
#include "pthread.h"
#include "crypt.h"
#include <string.h>
#include <math.h>
#include <time.h>

typedef struct Connection{
    rio_t rio;
    int connfd1;
    int connfd2;
    pthread_t *th1, *th2;
}Connection;

void* _recieve1(void* argv){
    Connection* conn = (Connection*) argv;
    char buf[MAXLINE];
    
    do {
        Rio_readinitb(&conn->rio, conn->connfd1);
        Rio_readlineb(&conn->rio, buf, MAXLINE);
        Rio_writen(conn->connfd2, buf, strlen(buf));
    }while (1);
    pthread_exit(NULL);
}

void* _recieve2(void* argv){
    Connection* conn = (Connection*) argv;
    char buf[MAXLINE];
    
    do {
        Rio_readinitb(&conn->rio, conn->connfd2);
        Rio_readlineb(&conn->rio, buf, MAXLINE);
        Rio_writen(conn->connfd1, buf, strlen(buf));
    }while (1);
    pthread_exit(NULL);
}

int main(int argc, char **argv) {
    int listenfd, connfd1, connfd2, port, clientlen;
    struct sockaddr_in clientaddr;
    struct hostent *hp;
    char *haddrp;
    
//-------------------------------------------------------------------------
    
//-----------------------------------------------------------------------
    
            port = atoi(argv[1]); /* the server listens on a port passed
                                   on the command line */
            listenfd = open_listenfd(port);
            char *host;
            Connection* conn = (Connection*) malloc(sizeof(Connection));
            do{
                printf("Waiting for connection....\n");
                clientlen = sizeof(clientaddr);
                connfd1 = Accept(listenfd, (SA *)&clientaddr, &clientlen);
                hp = Gethostbyaddr((const char *)&clientaddr.sin_addr.s_addr,
                               sizeof(clientaddr.sin_addr.s_addr), AF_INET);
                haddrp = inet_ntoa(clientaddr.sin_addr);
                conn->connfd1 = connfd1;
    
                printf("Waiting for connection....\n");
                printf("server connected to client 1 at %s (%s)\n", hp->h_name, haddrp);
                connfd2 = Accept(listenfd, (SA *)&clientaddr, &clientlen);
                hp = Gethostbyaddr((const char *)&clientaddr.sin_addr.s_addr,
                                   sizeof(clientaddr.sin_addr.s_addr), AF_INET);
                haddrp = inet_ntoa(clientaddr.sin_addr);
                conn->connfd2 = connfd2;
    
                printf("server connected to client 2 at %s (%s)\n", hp->h_name, haddrp);
                printf("type exit to quit\n");
    
                pthread_t th1, th2;
    
                conn->th1 = &th1;
                conn->th2 = &th2;
                pthread_create(&th1, NULL, _recieve1, conn);
                pthread_create(&th2, NULL, _recieve2, conn);
                pthread_join(th2, NULL);
        
            }while (1);
        
        Close(connfd1);
        Close(connfd2);
        free(conn);
}
