#include <stdio.h>
#include <stdlib.h>

int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;

	printf("%s\n", *envp);
//	printf("%s\n", getenv(argv[1]));
	return (0);
}

PATH=/home/labderra/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin
PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin
