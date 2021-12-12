#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include "server.h"
#include "settings.h"
#include "http.h"

# define SERVER_ERROR(fmt, ...) do { fprintf(stderr, "server error: " fmt "\n",##__VA_ARGS__); exit(-1); } while (0)

# define SERVER_INFO(fmt, ...) do { if (SRV_INFO) fprintf(stderr, "server info: " fmt "\n", ##__VA_ARGS__); } while (0)

int		server_init(char *ip, int port, int numofclients)
{
	int	lfd = socket(AF_INET, SOCK_STREAM, 0);
	if (lfd == -1)
	{
		SERVER_ERROR("cann't create a socket");
	}
	int	anyip = (in_addr_t *)ip == (in_addr_t *)INADDR_ANY;
	struct 	sockaddr_in	servaddr;
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = PF_INET;
	servaddr.sin_addr.s_addr = anyip ? INADDR_ANY : inet_addr(ip);
	servaddr.sin_port = htons(port);
	if (bind(lfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) == -1)
	{
		SERVER_ERROR("cann't bind socket to interface with IP: %s", anyip ? "any" : ip);
		exit(-1);
	}
	if (listen(lfd, numofclients) == -1)
		SERVER_ERROR("cann't create a queue (system call listen)");
	fprintf(stderr, "server was started IP: %s, PORT: %d\n", ip, port);
	return (lfd);
}


void	*server(void *shbuf) {
	fprintf(stderr, "server was started\r\n");
	struct s_shbuf *sharedbuf = (struct s_shbuf *) shbuf;
	int				lfd = server_init(IP, PORT, 1);
	int				cfd;

	while (1) 
	{
		cfd = accept(lfd, NULL, NULL);
		if (cfd == -1)
		{
			fprintf(stderr, "error: cann't accept client.\n");
			exit(-1);
		}
		else
			SERVER_INFO("client was connected");
		char *request = malloc(WEBBUF_SIZE);
		char *response = malloc(WEBBUF_SIZE);
		char *html = malloc(WEBBUF_SIZE);
		char *data;
		int f, datalen;

		while (1)
		{
 			memset(request, 0, WEBBUF_SIZE);
 			if (recv(cfd, request, WEBBUF_SIZE, 0) == -1) {
 				SERVER_INFO("connection was closed");
 				break;
 			} 
 			else
 				SERVER_INFO("client request:\n%s", request);
			memset(response, 0, WEBBUF_SIZE);
			switch (http_parse_request(request)) {
				case HTTP_REQ_INDEX_HTML:
					f = open(INDEXHTML_PATH, O_RDONLY);
					memset(html, 0, WEBBUF_SIZE);
					read(f, html, WEBBUF_SIZE);
					close(f);
					http_200OK(response, html);
				break;
				case HTTP_REQ_READDATA:
					pthread_mutex_lock(sharedbuf->mutex);
					http_200OK(response, sharedbuf->buf);
					memset(sharedbuf->buf, 0, sharedbuf->bufsize);
					pthread_mutex_unlock(sharedbuf->mutex);
				break;
				case HTTP_REQ_WRITEDATA:
					datalen = http_find_data (request, &data);
					if (datalen != -1) {
						data[datalen] = '\n';
						data[datalen] = '\0';
						write(STDOUT_FILENO, data, datalen+2);
					}
					http_200OK(response, "");
				break;
				case HTTP_REQ_OTHER:
					http_200OK(response, "");
				break;
			}
			if (send(cfd, response, strlen(response), 0) == -1) {
 				SERVER_INFO("connection was closed");
				break;
			} else
				SERVER_INFO("server response:\n%s", response);
 		}
		/*
		pthread_mutex_lock(sharedbuf->mutex); // захватили мьютекс
		if (strlen(sharedbuf->buf) != 0) 
		{
			fprintf(stderr, "%s\r\n", sharedbuf->buf); // выводим их на экран
			memset(sharedbuf->buf, 0, sharedbuf->bufsize); // и очищаем буфер
		}
		pthread_mutex_unlock(sharedbuf->mutex); // освободили мьютекс
		*/
		// close client connection:
		free (request);
		free (response);
		close (cfd);
		}		
	fprintf(stderr, "server was finished\r\n");
	return (NULL);
}

