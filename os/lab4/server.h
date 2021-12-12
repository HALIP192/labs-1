#ifndef SERVER_H
# define SERVER_H
# include <pthread.h>

typedef	struct 	s_shbuf
{
	char			*buf;
	size_t			bufsize;
	pthread_mutex_t	*mutex;	
}	t_shbuf;

extern void	*server(void *shbuf);

#endif