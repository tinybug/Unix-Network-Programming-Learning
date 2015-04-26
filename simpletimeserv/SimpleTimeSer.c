#include "common.h"
#include <time.h>

int main(int argc, char **argv)
{
    int listenfd = Socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(13);

    Bind(listenfd, (SA *)&servaddr, sizeof(servaddr));

    Listen(listenfd, LISTENQ);

    int connfd;
    time_t ticks;
    char buff[MAXLINE];
    for(;;)
    {
        connfd = Accept(listenfd, (SA *) NULL, NULL);
        ticks = time(NULL);
        snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
        Writen(connfd, buff, strlen(buff));
        Close(connfd);
    }

    return 0;
}
