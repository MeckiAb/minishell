#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <readline/readline.h>
#include "../libft/libft.h"

int	glob = 0;

void	handle_sigint_parent(int sig)
{
	(void)sig;
	printf("\n");
}

void	handle_sigint_main(int sig)
{
	(void)sig;
	printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

int	main(int argc, char **argv, char **envp)
{
	char	*str;
	int		pid;
	int		status;
	char	**split;

	(void)argv;
	if (argc != 1)
		return (1);
	while (1)
	{
		signal(SIGINT, handle_sigint_main);
		str = readline("cosa : ");
		if (!str)
			break;
		split = ft_split(ft_strjoin("/usr/bin/", str), ' ');
		pid = fork();
		if (!pid)
		{
			execve(split[0], split, envp);
			perror("execve");
			exit(127);
		}
		else
		{
			signal(SIGINT, handle_sigint_parent);
			waitpid(pid, &status, 0);
		}
		free(str);
	}
	return (0);
}