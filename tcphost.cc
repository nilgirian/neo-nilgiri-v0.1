/* **************************************************************************
    Nilgiri the Forgotten World
    (C) 2004
    tcphost.cc: server socket setup and communication
   ************************************************************************** */
/** \file
    \brief Server socket setup and communication source
 */

#include<stdlib.h>
#include<unistd.h>
#include<netdb.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/telnet.h>
#include<fcntl.h>
#include<sys/param.h>
#include<errno.h>
#include<string.h>
#include<stdio.h>
#include"err.h"
#include"tcphost.h"
#include"addlog.h"

/** File descriptor of the server.  This should only be set once by
    init_tcphost_module() and never be changed throughout the lifetime of the
    program.  This should be the last init function to be instantiated.
 */
int tcphostfd = -1;
/// the current number of descriptors
static int num_descs = 0;

const char echo_on[] =
{
	'\r',
	'\n',
	IAC,
	WONT,
	TELOPT_ECHO,
	'\0'
};

const char echo_off[] =
{
	IAC,
	WILL,
	TELOPT_ECHO,
	'\0'
};

/** initializes the tcphost module, and starts the server
    \param hostname where to run the server
    \param port what port to use
 */
void init_tcphost_module(const char *hostname, const unsigned short int port)
{
	struct hostent *host;
	struct sockaddr_in sock;
	int opt;

	fprintf(stderr, "Nilgiri now running on %s port %u\n", hostname, port);
	addlog_host(hostname, port);

	if ((host = gethostbyname(hostname))==NULL)
	{
		exitlog("/gethostbyname/%s", hstrerror(h_errno));
	}

	if ((tcphostfd = socket(AF_INET, SOCK_STREAM, 0))==-1)
	{
		exitlog("/socket/%s", strerror(errno));
	}

	opt = 1;
	if (setsockopt(tcphostfd, SOL_SOCKET, SO_REUSEADDR,
	(void *) &opt, sizeof(opt))==-1)
	{
		exitlog("/setsockopt/%s", strerror(errno));
	}

	memset(&sock, 0, sizeof(struct sockaddr_in));
	sock.sin_family = host->h_addrtype;
	sock.sin_port = htons(port);

	if (bind(tcphostfd, (struct sockaddr *) &sock, sizeof(sock))==-1)
	{
		exitlog("/bind/%s", strerror(errno));
	}

	if (listen(tcphostfd, 16)==-1)
	{
		exitlog("/listen/%s", strerror(errno));
	}

	if (fcntl(tcphostfd, F_SETFL, O_NONBLOCK)==-1)
	{
		exitlog("/fcntl/%s", strerror(errno));
	}
}

/** Runs gethostbyaddr() on an ip and returns the name of the host.  This
    function can block.
    \param ip The ip address to lookup
    \return the name of ip address
 */
const char *get_hostname(unsigned long int ip)
{
	struct hostent *h;
	if ((h = gethostbyaddr((char *) &ip, (socklen_t) sizeof(ip), AF_INET))==NULL)
	{
		return NULL;
	}
	else
	{
		return h->h_name;
	}
}

/** retrieves a file descriptor for a new connection to the socket
    \param ip pointer to an ip field which will be populated by the client side
     ip value
    \return the file descriptor
 */
int new_connection(unsigned long int *ip)
{
	struct sockaddr_in sock;
	socklen_t socksize;
	int desc;

	socksize = (socklen_t) sizeof(sock);
	getsockname(tcphostfd, (struct sockaddr *) &sock, &socksize);

	if ((desc = accept(tcphostfd, (struct sockaddr *) &sock, &socksize)) == -1)
	{
		return -1;
	}

	if (fcntl(desc, F_SETFL, O_NONBLOCK)==-1)
	{
		errlog("/fcntl/%s", strerror(errno));
		return -1;
	}

	*ip = (unsigned long int) sock.sin_addr.s_addr;

	num_descs++;

	return desc;
}

/** closes file descriptor the connection to the socket
    \param desc the file descriptor to close
 */
void destroy_connection(int desc)
{
	close(desc);
	num_descs--;
}

/** retrives the count of the number of current file descriptors in use by the
    socket
    \return the number of descriptors
 */
int get_num_descs(void)
{
	return num_descs;
}

/** handles writing messages to a particular file descriptor on the socket
    \param desc the descriptor
    \param msg the message to write
    \return -2 on EPIPE, -1 on error, 0 on success, 1 on EAGAIN
 */
int tcpwrite(int desc, char *msg)
{
	int len;

	len = (int) strlen(msg)+1;

	if (len != send(desc, msg, len, MSG_NOSIGNAL))
	{
		switch(errno)
		{
			case EAGAIN:
				return 1;
			case EPIPE:
				return -2;
			default:
				errlog("/send/%s", strerror(errno));
				return -1;
		}
	}
	return 0;
}

/** handles general reading from a socket for a particular file descriptor
    \param desc the descriptor
    \param msg pointer to where to write the read message
    \param msglen the number of bytes read on this instantiation
    \param maxlen the maximum number of bytes to read for any particular msg
    \return -2 on EPIPE, -1 on error
             0 on success, 1 on EAGAIN, 2 on maxlen, 3 on close
 */
int tcpread(int desc, char *msg, size_t *msglen, size_t maxlen)
{
	static size_t chunk_size = CHUNK_SIZE;
	int cnt;
	size_t chunk;

	maxlen--;

	if (*msglen > maxlen - chunk_size)
	{
		chunk = maxlen - *msglen;
		if (chunk==0)
		{
			msg[*msglen] = '\0';
			return 2;
		}
	}
	else
	{
		chunk = chunk_size;
	}

	while ((cnt = recv(desc, msg+*msglen, chunk, MSG_NOSIGNAL)) > 0)
	{

		if (cnt == -1)
		{
			switch(errno)
			{
				case EAGAIN:
					return 1;
				case EPIPE:
					return -2;
				default:
					errlog("/recv/%s", strerror(errno));
					return -1;
			}
		}
		*msglen += (size_t) cnt;

		if (*msglen > maxlen - chunk_size)
		{
			chunk = maxlen - *msglen;
			if (chunk==0)
			{
				msg[*msglen] = '\0';
				return 2;
			}
		}
	}
	if (cnt == 0)
	{
		return 3;
	}
	msg[*msglen] = '\0';
	if (msg[*msglen-1] == '\n')
	{
		return 0;
	}
	else
	{
		return 1;
	}
}
