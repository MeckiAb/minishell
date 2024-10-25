/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labderra <labderra@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 14:42:10 by labderra          #+#    #+#             */
/*   Updated: 2024/10/25 07:58:57 by labderra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	apply_redir(t_command *cmd)
{
	if (cmd->infile != -2)
	{
		dup2(cmd->infile, STDIN_FILENO);
		close(cmd->infile);
	}
	if (cmd->outfile != -2)
	{
		dup2(cmd->outfile, STDOUT_FILENO);
		close(cmd->outfile);
	}
}

void	revert_redir(t_mini *mini, t_command *cmd)
{
	(void)cmd;
	dup2(mini->mini_in, STDIN_FILENO);
	dup2(mini->mini_out, STDOUT_FILENO);
}
