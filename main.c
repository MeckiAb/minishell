/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labderra <labderra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 12:45:50 by labderra          #+#    #+#             */
/*   Updated: 2024/09/17 12:09:25 by labderra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_mini	*init_shell(char **argv, char **envp)
{
	t_mini	*mini;

	mini = malloc(sizeof(t_mini));
	if (!mini)
		return (NULL);
	mini->argv = argv;
	mini->envp = envp;
	mini->path = get_full_path(envp);
	if (!mini->path)
		return (NULL);
	mini->status = 0;
	mini->tkn_list = NULL;
	return (mini);
}

void handle_sigint(int sig)
{
	(void)sig;
	printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void handle_sigquit(int sig)
{
	(void)sig;
	printf("Quit (core dumped)\n");
	exit(131);
}

int	main(int argc, char **argv, char **envp)
{
	char		*str;
	t_mini		*mini;

	(void)argv;
	mini = init_shell(argv, envp);
	if (argc != 1 || !mini)
		return (1);
	signal(SIGINT, handle_sigint);
	//signal(SIGQUIT, handle_sigquit);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		str = readline("PorongaShell :\\>");
		if (!str)
		{
			printf("%s", "exit\n");
			break ;
		}
		add_history(str);
		parse_line(mini, str);
		exec_line(mini);
		free_list(&(mini->tkn_list));
		free(str);
	}
	free_split(mini->path);
	free(mini);
	return (0);
}
