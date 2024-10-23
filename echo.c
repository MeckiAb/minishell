/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labderra <labderra@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 00:11:07 by labderra          #+#    #+#             */
/*   Updated: 2024/10/24 00:19:18 by labderra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	run_echo(t_mini *mini, t_command *cmd)
{
	int	n_flag;
	int	i;

	apply_redir(cmd);
	i = 0;
	n_flag = 0;
	if (cmd->arg_array[1] && cmd->arg_array[1][i++] == '-')
	{
		while (cmd->arg_array[1][i] == 'n')
			i += 1;
		n_flag = cmd->arg_array[1][i] == '\0';
	}
	i = 1 + n_flag;
	while (cmd->arg_array[i])
	{
		write(1, cmd->arg_array[i], ft_strlen(cmd->arg_array[i]));
		if (cmd->arg_array[++i])
			write(1, " ", 1);
	}
	if (n_flag == 0)
		write(1, "\n", 1);
	revert_redir(mini, cmd);
	return (0);
}
