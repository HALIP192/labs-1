#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>

int				clk_button;
pthread_mutex_t	mutex = PTHREAD_MUTEX_INITIALIZER; 

int	ft_input_stdin(void)
{
	char	arr[20];
	int		value;
	size_t	len;

	len = 30;
	value = -1;
	while (1)
	{
		read(0, arr, 20);
		value = atoi(arr + 6);
		if (value < 0 && value > 3)
		{
			printf("Error. Button is wrong");
			continue ;
		}
		pthread_mutex_lock(&mutex);
		clk_button = value;
		pthread_mutex_unlock(&mutex);
		if (value == 3)
			break ;
	}
	pthread_exit(0);
}

void	ft_init(char *arr)
{
	clk_button = 0;
	strncpy(arr, "0101", 4);
}

void	ft_putchr(char c)
{
	write(1, &c, 1);
}

void	ft_printf_led(char *str)
{
	for (int i = 0; i < 4; i++)
	{
		write(1, "LED", 3);
		ft_putchr(i + '0');
		if (str[i] == '0')
			write(1, ": gray\n", 7);
		else
			write(1, ": green\n", 8);
	}
	write(1, "\n", 1);
}

void	ft_inc_value(int *value)
{
	(*value)++;
	if (*value > 15)
		(*value) = 0;
}

void	ft_dec_value(int *value)
{
	(*value)--;
	if (*value < 0)
		(*value) = 15;
}

void	ft_clear_value(int *value)
{
	*value = 5;
}

void	ft_tobin(char *arr, int value)
{
	size_t	i;

	i = 3;
	strncpy(arr, "0000", 4);
	while (value > 0)
	{
		arr[i--] += value % 2;
		value /= 2;
	}
}

int	main(int argc, char **argv)
{
	char		leds[4];
	int			value;
	void		(*ptr_func)(int *);
	pthread_t	thread;

	ptr_func = NULL;
	value = 5;
	ft_init(leds);
	if (pthread_create(&thread, NULL, (void *)ft_input_stdin, NULL) != 0)
	{
		printf("Error. Don't can  create new pthread\n");
		exit(-1);
	}
	while (1)
	{
		pthread_mutex_lock(&mutex);
		if (clk_button == 0)
		{
			ptr_func = ft_inc_value;
		}
		else if (clk_button == 1)
			ptr_func = ft_dec_value;
		else if (clk_button == 2)
			ptr_func = ft_clear_value;
		else if (clk_button == 3)
			break ;
		pthread_mutex_unlock(&mutex);
		(*ptr_func)(&value);
		ft_tobin(leds, value);
		ft_printf_led(leds);
		usleep(1000000);
	}
	if (pthread_join(thread, NULL) != 0)
	{
		printf("Error. Pthread child don't finished\n");
		exit(-1);
	}
	pthread_mutex_destroy(&mutex);
	return (0);
}
