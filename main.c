/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labderra <labderra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 12:45:50 by labderra          #+#    #+#             */
/*   Updated: 2024/09/11 13:26:14 by labderra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	char		*str;
	t_mini		*mini;

	(void)argv;
	mini = init_shell(envp);
	if (argc != 1 || !mini)
		return (1);
	while (1)
	{
		str = readline("MeckiShell :\\>");
		add_history(str);
		parse_line(mini, str);
		exec_line(mini);
		free_list(mini->tkn_list);
		free(str);
	}
	free_split(mini->envp);
	free_split(mini->path);
	return (0);
}
