#include <stdio.h>
#include <unistd.h>

int	main(void)
{
	int	button;

	while (1)
	{
		scanf("%d", &button);
		if (button > 2)
			return (0);
		write(1, "BUTTON", 6);
		write(1, button == 0? "0" : (button == 1 ? "1" : "2"), 1);
		write(1, ": clicked\n", 10);
	}
	return (0);
}
