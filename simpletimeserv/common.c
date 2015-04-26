#include "common.h"

int daemon_proc = 0;

static void	err_doit(int, int, const char *, va_list);

void err_sys(const char *fmt, ...)
{
	va_list	ap;

	va_start(ap, fmt);
	err_doit(1, LOG_ERR, fmt, ap);
	va_end(ap);
	exit(1);
}

void err_quit(const char *fmt, ...)
{
	va_list	ap;

	va_start(ap, fmt);
	err_doit(0, LOG_ERR, fmt, ap);
	va_end(ap);
	exit(1);
}

static void err_doit(int errnoflag, int level, const char *fmt, va_list ap)
{
	int		errno_save, n;
	char	buf[MAXLINE];

	errno_save = errno;
#ifdef	HAVE_VSNPRINTF
	vsnprintf(buf, sizeof(buf), fmt, ap);
#else
	vsprintf(buf, fmt, ap);
#endif
	n = strlen(buf);
	if (errnoflag)
	{
        snprintf(buf+n, sizeof(buf)-n, ": %s", strerror(errno_save));
	}
	strcat(buf, "\n");

	if (daemon_proc) {
		syslog(level, buf);
	} else {
		fflush(stdout);
		fputs(buf, stderr);
		fflush(stderr);
	}
	return;
}

int Socket(int domain, int type, int protocol)
{
    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        err_sys("socket error\n");
    }
    return sockfd;
}

void Bind(int fd, const SA * addr, socklen_t len)
{
    if (bind(fd, addr, len) < 0)
    {
        err_sys("bind error");
    }
}

void Listen(int fd, int n)
{
    char *ptr;
    if ( (ptr = getenv("LISTENQ")) != NULL)
    	n = atoi(ptr);
    if (listen(fd,  n) < 0)
    	err_sys("listen error");
}

int Accept(int fd, SA * addr, socklen_t * addr_len)
{
	int	n;

again:
	if ( (n = accept(fd, addr, addr_len)) < 0) {
#ifdef	EPROTO
		if (errno == EPROTO || errno == ECONNABORTED)
#else
		if (errno == ECONNABORTED)
#endif
			goto again;
		else
			err_sys("accept error");
	}
	return(n);
}

ssize_t	writen(int fd, const void *vptr, size_t n)
{
	size_t nleft;
	ssize_t nwritten;
	const char *ptr;

	ptr = vptr;
	nleft = n;
	while (nleft > 0) {
		if ( (nwritten = write(fd, ptr, nleft)) <= 0) {
			if (errno == EINTR)
				nwritten = 0;
			else
				return(-1);
		}

		nleft -= nwritten;
		ptr   += nwritten;
	}
	return(n);
}

void Writen(int fd, void *ptr, size_t nbytes)
{
    if (writen(fd, ptr, nbytes) != nbytes)
        err_sys("writen error");
}

void Close(int fd)
{
    if (close(fd) == -1)
        err_sys("close error");
}

