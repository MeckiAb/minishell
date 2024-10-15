#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include "../libft/libft.h"

int	glob = 0;

void	handle_sigint(int sig)
{
	(void)sig;
	if (glob)
	{
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		glob = 0;
	}
	else
		printf("/n");
}

int	main(int argc, char **argv, char **envp)
{
	char	*str;
	int		pid;
	int		status;
	char	**split;

	(void)argv;
	if (argc != 2)
		return (1);
	signal(SIGINT, handle_sigint);
	while (1)
	{
		glob = 0;
		str = readline("cosa : ");
		if (!str)
			break;
		split = ft_split(ft_strjoin("/usr/bin/", str), ' ');
		printf("%s - %s\n", split[0], split[1]);
		pid = fork();
		if (!pid)
		{
			printf("hijo\n");
			execve(split[0], split, envp);
			perror("execve");
		}
		else
		{
			glob = 1;
			waitpid(pid, &status, 0);
		}
	}
	return (0);
}