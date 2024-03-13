#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <sys/types.h>
#include <limits.h>
#include <stdio.h>
#include <stdbool.h>
#include <fcntl.h>

#define FATAL "Fatal error"
#define BUFFER_SIZE 1024

void error(const char *message) {
    write(STDERR_FILENO, message, strlen(message));
    write(STDERR_FILENO, "\n", 1);
    exit(1);
}

void fail(fd_set *fds, int fdmax, char **msg) {
    for (int i = 0; i <= fdmax; ++i) {
        if (FD_ISSET(i, fds)) {
            close(i);
            free(msg[i]);
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
    if (argc <= 1) {
        error("Wrong number of arguments");
    }

	// socket create and verification
    int server;
	if ((server = socket(AF_INET, SOCK_STREAM, 0)) == -1) { 
		error(FATAL);
	}

	struct sockaddr_in servaddr;
	bzero(&servaddr, sizeof(servaddr)); 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = htonl(2130706433); //127.0.0.1
	servaddr.sin_port = htons(atoi(argv[1])); 
  
	// Binding newly created socket to given IP and verification 
	if ((bind(server, (const struct sockaddr *)&servaddr, sizeof(servaddr))) != 0) {
        close(server);
		error(FATAL);
	}
	if (listen(server, 10) != 0) {
        close(server);
		error(FATAL);
	}

    fcntl(server, F_SETFL, O_NONBLOCK); // TESTING

    fd_set fds;
    fd_set read_fds;
    FD_ZERO(&fds);
    FD_SET(server, &fds);
    int fdmax = server;
    int last_client = -1;
    int ids[FD_SETSIZE] = {-1};
    char *msg[FD_SETSIZE] = {NULL};
    char buf[BUFFER_SIZE];

    while (true) {
        read_fds = fds;
        if (select(fdmax + 1, &read_fds, NULL, NULL, NULL) < 0) {
            fail(&fds, fdmax, msg);
        }
        for (int i = 0; i <= fdmax; ++i) {
            if (FD_ISSET(i, &read_fds)) {
                if (i == server) { // new client connection
                    struct sockaddr_in client_addr;
                    socklen_t addr_len = sizeof(client_addr);
                    int client = accept(server, (struct sockaddr *)&client_addr, &addr_len);
                    if (client > -1) {
                        FD_SET(client, &fds);
                        if (client > fdmax) {
                            fdmax = client;
                        }
                        ids[client] = ++last_client;
                        bzero(&buf, BUFFER_SIZE);
                        sprintf(buf, "server: client %d just arrived\n", ids[client]);
                        send_message(&fds, server, client, fdmax, buf);
                        // printf("%s", buf);
                    }
                } else { // read client
                    bzero(&buf, BUFFER_SIZE);
                    ssize_t read_bytes;
                    if ((read_bytes = recv(i, &buf, BUFFER_SIZE - 1, 0)) == 0) {
                        // client closed connection
                        FD_CLR(i, &fds);
                        close(i);
                        if (ids[i] == last_client) {
                            last_client--;
                        }
                        bzero(&buf, BUFFER_SIZE);
                        sprintf(buf, "server: client %d just left\n", ids[i]);send_message(&fds, server, i, fdmax, buf);
                        // printf("%s", buf);
                        ids[i] = -1;
                        free(msg[i]);
                        msg[i] = NULL;
                    } else if (read_bytes > 0) {
                        // printf("reading from client\n");
                        if ((msg[i] = str_join(msg[i], buf)) == NULL) {
                            fail(&fds, fdmax, msg);
                        }
                        char *line = NULL;
                        int res;
                        while ((res = extract_message(&msg[i], &line)) > 0) {
                            char *temp = calloc(15 + BUFFER_SIZE, sizeof(char));
                            if (temp == NULL) {
                                free(line);
                                fail(&fds, fdmax, msg);
                            }
                            sprintf(temp, "client %d: ", ids[i]);
                            temp = str_join(temp, line);
                            free(line);
                            if (temp == NULL) {
                                fail(&fds, fdmax, msg);
                            }
                            send_message(&fds, server, i, fdmax, temp);
                            // printf("%s", temp);
                        }
                        if (res == -1) {
                            fail(&fds, fdmax, msg);
                        }
                    }
                }
            }
        }
    }
    return (1);
}
