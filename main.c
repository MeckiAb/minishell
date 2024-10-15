/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labderra <labderra@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 12:45:50 by labderra          #+#    #+#             */
/*   Updated: 2024/10/15 23:43:06 by labderra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	global_signal;

static t_mini	*init_shell(char **argv, char **envp)
{
	t_mini	*mini;

	mini = malloc(sizeof(t_mini));
	if (!mini)
		return (NULL);
	mini->argv = argv;
	mini->envp = copy_environment(envp);
	mini->envp_dictionary = copy_split_environment(mini->envp);
	mini->path = get_full_path(mini->envp);
	if (!mini->path)
		return (NULL);
	mini->status = 0;
	mini->mini_in = dup(STDIN_FILENO);
	mini->mini_out = dup(STDOUT_FILENO);
	mini->tkn_list = NULL;
	mini->cmd_list = NULL;
	return (mini);
}

void	free_dictionary(char ***dict)
{
	char	***p;

	p = dict;
	while (p && *p)
	{
		free((*p)[0]);
		free((*p)[1]);
		free(*p++);
	}
	free(dict);
}

void	free_shell(t_mini *mini)
{
	free_commands_and_tokens(mini);
	rl_clear_history();
	free_split(mini->path);
	free_split(mini->envp);
	free_dictionary(mini->envp_dictionary);
	close(mini->mini_in);
	close(mini->mini_out);
	free(mini);
}

void	handle_sigint_fork(int sig)
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
	char		*str;
	t_mini		*mini;

	mini = init_shell(argv, envp);
	init_environment(mini, envp);
	if (argc != 1 || !mini)
		return (1);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		signal(SIGINT, handle_sigint_main);
		str = readline("MiniShell :\\>");
		if (!str)
		{
			printf("exit\n");
			break ;
		}
		add_history(str);
		lexer(mini, str);
		parser(mini);
		exec_line(mini);
		free_commands_and_tokens(mini);
		free(str);
	}
	free_shell(mini);
	return (0);
}
