/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labderra <labderra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 13:43:23 by labderra          #+#    #+#             */
/*   Updated: 2024/10/23 13:46:30 by labderra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_list(t_tkn **tkn_list)
{
	t_tkn	*tmp;

	while (tkn_list && *tkn_list)
	{
		tmp = *tkn_list;
		*tkn_list = (*tkn_list)->next;
		free(tmp->tkn);
		free(tmp);
	}
	*tkn_list = NULL;
}

void	free_split(char **str)
{
	int	i;

	i = 0;
	while (str && str[i])
		free(str[i++]);
	if (str)
		free(str);
}

void	free_commands_and_tokens(t_mini *mini)
{
	t_tkn		*tkn;
	t_command	*cmd;

	while (mini->tkn_list)
	{
		tkn = mini->tkn_list;
		mini->tkn_list = mini->tkn_list->next;
		free(tkn->tkn);
		free(tkn);
	}
	while (mini->cmd_list)
	{
		cmd = mini->cmd_list;
		mini->cmd_list = mini->cmd_list->next;
		free_split(cmd->arg_array);
		free(cmd);
	}
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
	free_split(mini->envp);
	free_dictionary(mini->envp_dictionary);
	close(mini->mini_in);
	close(mini->mini_out);
	free(mini);
}
