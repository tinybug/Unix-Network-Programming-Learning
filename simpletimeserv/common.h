#ifndef	__common_h
#define	__common_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdarg.h>
#include <syslog.h>
#include <errno.h>

#define	MAXLINE	4096

#define	LISTENQ	1024

#define	SA struct sockaddr

extern int daemon_proc;

void    err_sys(const char * fmt, ...);
void    err_quit(const char * fmt, ...);
int     Socket(int domain, int type, int protocol);
void	Bind(int fd, const SA * addr, socklen_t len);
void	Listen(int fd, int n);
int	Accept(int fd, SA * addr, socklen_t * addr_len);
void	Writen(int fd, void * ptr, size_t nbytes);
void    Close(int fd);

#endif
