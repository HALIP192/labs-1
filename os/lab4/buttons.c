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
		if (button == 0)
			write(1, "BUTTON0: clicked\n", 17);
		else if (button == 1)
			write(1, "BUTTON1: clicked\n", 17);
		else
			write(1, "BUTTON2: clicked\n", 17);
	}
	return (0);
}
