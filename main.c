/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labderra <labderra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 12:45:50 by labderra          #+#    #+#             */
/*   Updated: 2024/10/18 13:50:35 by labderra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_signal;

int	main(int argc, char **argv, char **envp)
{
	char		*str;
	t_mini		*mini;

	mini = init_shell(argv, envp);
	if (argc != 1 || !mini)
		return (1);
	while (1)
	{
		signal(SIGQUIT, SIG_IGN);
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
