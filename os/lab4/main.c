#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "server.h"
#include "settings.h"

int		main(void)
{
	pthread_t	thread;
	t_shbuf		*sbuff;
	char		buff[STDIN_DATA_SIZE + 1];
	size_t		len_buf;

	sbuff = (sbuf *)malloc(sizeof(t_shbuf));
	sbuf->bufsize = SHAREDBUF_SIZE;
	sbuf->buf = (char *)malloc(sizeof(char) * sbuf->bufsize);
	memset(sbuf->buf, 0, sbuf->bufsize);
	memset(buff, 0, STDIN_DATA_SIZE);
	sbuf->mutex = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
	printf("main was started\n");
	if (ptread_create(&thread, NULL, server, NULL) != 0)
	{
		fprintf(stderr, "error: pthread_create was failed.\n");
		return (-1);
	}
	while (1)
	{
		len_buf = read(0, buff, STDIN_DATA_SIZE);
		buff[len_buf] = '\0';
		pthread_mutex_lock(sbuf->mutex);
		if ((sbuf->bufsize - strlen(sbuf->buf) - 1) > len_buf)
			strcat(sbuf->buf, buff);
		pthread_mutex_unlock(sbuf->mutex);
	}
	if (pthread_join(thread, NULL) != 0)
	{
		fprintf(stderr,"error: pthread_join was failed.\n");
		return (-1);
	}
	free(sbuf->buf);
	free(sbuf->mutex);
	free(sbuf);

	printf("main and server was finished\n");
	return (0);
}