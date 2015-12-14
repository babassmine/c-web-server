#include "csapp.h"
#include "pthread.h"
#include "crypt.h"
#include <string.h>
#include <math.h>
#include <time.h>

int main(int argc, char **argv) {
    int listenfd, connfd, port, clientlen;
    struct sockaddr_in clientaddr;
    struct hostent *hp;
    char *haddrp;

    if (atoi(argv[1])==0)
        port = 80
    else
     port = atoi(argv[1]); /* the server listens on a port passed
                             on the command line */
    listenfd = open_listenfd(port);
    rio_t rio;

    char* buf = (char*) malloc(1024);
    while (1) {
        clientlen = sizeof(clientaddr);
        connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
        hp = Gethostbyaddr((const char *)&clientaddr.sin_addr.s_addr,
                        sizeof(clientaddr.sin_addr.s_addr), AF_INET);
        haddrp = inet_ntoa(clientaddr.sin_addr);
        printf("server connected to %s (%s)\n", hp->h_name, haddrp);
                Rio_readinitb(&rio, connfd);
                Rio_readlineb(&rio, buf, MAXLINE);
                printf("Echo:");
                Fputs(buf, stdout);
                char* filename;
                strtok(buf, " ");
                filename = strtok(NULL, " ");
                printf("%s\n", filename);
                _Read(filename, connfd);
//        echo(connfd);
        free(buf);
                Close(connfd);
    }
    exit(0)
}
