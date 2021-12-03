#include <stdio.h>
#include <pthread.h>
#include <string.h>

int	clk_button;

int	ft_input_stdin()
{

	while (1)
	{
		// need lock mem
		scanf("button%d: clicked", clk_button);
		if (clk_button < 0 && clk_button < 3)
		{
			printf("Error. Button is wrong");
			continue;
		}
		// need unlock mem

	}
	return (0);
}


void	ft_init(char *arr)
{
	clk_button = -1;
	strncpy(arr, "101", 3);
}

void	ft_printf_led(char *str)
{
	write(1, str, 3);
}

void	ft_inc_value(int *value)
{
	(*value)++;
}

void	ft_dec_value(int *value)
{
	(*value)--;
}

int	main(int argc, char **argv)
{
	char	leds[3];
	char	value;
	int		*ptr_value(int *);

	value = 0;
	ft_init(leds);
	while (1)
	{
		// check mem
		if ()
		{
			if (clk_button == 0)
				ptr_func = ft_inc_value;
			else if (clk_button == 1)
				ptr_func = ft_dec_value;
			else if (clk_button == 2)
			{
				value = 0;
				continue;
			}
		}
		// lock mem
		ptr_func(&value);
		ft_tobin(leds, value);
		ft_printf_led(leds);
		// unlock mem
	}
	return (0);
}
