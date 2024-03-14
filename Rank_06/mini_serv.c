#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>

#define FATAL "Fatal error"
#define BUFFER_SIZE 1024

void error(const char *msg) {
    write(STDERR_FILENO, msg, strlen(msg));
    write(STDERR_FILENO, "\n", 1);
    exit(1);
}

void fail(fd_set *fds, char **msg, int fdmax) {
    for (int i = 0; i <= fdmax; ++i) {
        if (FD_ISSET(i, fds)) {
            free(msg[i]);
            close(i);
        }
    }
    error(FATAL);
}

void send_message(fd_set *fds, int server, int client, int fdmax, char *msg) {
    for (int i = 0; i <= fdmax; ++i) {
        if (FD_ISSET(i, fds) && i != server && i != client) {
            send(i, msg, strlen(msg), 0);
        }
    }
}

int extract_message(char **buf, char **msg)
{
	char	*newbuf;
	int	i;

	*msg = 0;
	if (*buf == 0)
		return (0);
	i = 0;
	while ((*buf)[i])
	{
		if ((*buf)[i] == '\n')
		{
			newbuf = calloc(1, sizeof(*newbuf) * (strlen(*buf + i + 1) + 1));
			if (newbuf == 0)
				return (-1);
			strcpy(newbuf, *buf + i + 1);
			*msg = *buf;
			(*msg)[i + 1] = 0;
			*buf = newbuf;
			return (1);
		}
		i++;
	}
	return (0);
}

char *str_join(char *buf, char *add)
{
	char	*newbuf;
	int		len;

	if (buf == 0)
		len = 0;
	else
		len = strlen(buf);
	newbuf = malloc(sizeof(*newbuf) * (len + strlen(add) + 1));
	if (newbuf == 0)
		return (0);
	newbuf[0] = 0;
	if (buf != 0)
		strcat(newbuf, buf);
	free(buf);
	strcat(newbuf, add);
	return (newbuf);
}

int main(int argc, char **argv) {
    if (argc < 2) {
        error("Wrong number of arguments");
    }

	struct sockaddr_in servaddr, cli; 
    socklen_t len = sizeof(cli);

	// socket create and verification 
	int server = socket(AF_INET, SOCK_STREAM, 0); 
	if (server == -1) { 
		error(FATAL);
	}

	bzero(&servaddr, sizeof(servaddr)); 
	// assign IP, PORT 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = htonl(2130706433); //127.0.0.1
	servaddr.sin_port = htons(atoi(argv[1]));
  
	// Binding newly created socket to given IP and verification 
	if ((bind(server, (const struct sockaddr *)&servaddr, sizeof(servaddr))) != 0 || listen(server, 10) != 0) {
        close(server);
        error(FATAL);
    }

    fd_set fds;
    fd_set events;
    FD_ZERO(&fds);
    FD_SET(server, &fds);
    int fdmax = server;
    int last = -1;
    int ids[FD_SETSIZE] = {-1};
    char *msg[FD_SETSIZE] = {NULL};
    char buf[BUFFER_SIZE];

    while (1) {
        events = fds;
        if (select(fdmax + 1, &events, NULL, NULL, NULL) == -1) {
            fail(&fds, msg, fdmax);
        }
        for (int i = 0; i <= fdmax; ++i) {
            if (FD_ISSET(i, &events)) {
                if (i == server) { // new connection
                    int client = accept(server, (struct sockaddr *)&cli, &len);
                    if (client >= 0) {
                        FD_SET(client, &fds);
                        ids[client] = ++last;
                        if (client > fdmax) {
                            fdmax = client;
                        }
                        bzero(buf, BUFFER_SIZE);
                        sprintf(buf, "server: client %d just arrived\n", last);
                        send_message(&fds, server, client, fdmax, buf);
                    }
                } else { // client event
                    ssize_t ret;
                    bzero(buf, BUFFER_SIZE);
                    if ((ret = recv(i, buf, BUFFER_SIZE - 1, 0)) == 0) {
                        // client closed connection
                        FD_CLR(i, &fds);
                        close(i);
                        free(msg[i]);
                        msg[i] = NULL;
                        if (i == fdmax) {
                            --fdmax;
                        }
                        bzero(buf, BUFFER_SIZE);
                        sprintf(buf, "server: client %d just left\n", ids[i]);
                        ids[i] = -1;
                        send_message(&fds, server, i, fdmax, buf);
                    } else if (ret > 0) { // client read send
                        msg[i] = str_join(msg[i], buf);
                        if (msg[i] == NULL) {
                            fail(&fds, msg, fdmax);
                        }
                        char *line = NULL;
                        while ((ret = extract_message(&msg[i], &line)) > 0) {
                            char *temp = calloc(30, sizeof(char));
                            if (temp == NULL) {
                                free(line);
                                fail(&fds, msg, fdmax);    
                            }
                            sprintf(temp, "client %d: ", ids[i]);
                            temp = str_join(temp, line);
                            free(line);
                            if (temp == NULL) {
                                fail(&fds, msg, fdmax);
                            }
                            line = NULL;
                            send_message(&fds, server, i, fdmax, temp);
                            free(temp);
                        }
                        if (ret == -1) {
                            fail(&fds, msg, fdmax);
                        }
                    }
                }
            }
        }
    }
    return (1);
}
