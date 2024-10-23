/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labderra <labderra@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 00:09:01 by labderra          #+#    #+#             */
/*   Updated: 2024/10/24 00:09:52 by labderra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	run_env(t_mini *mini, t_command *cmd)
{
	char	**aux;

	apply_redir(cmd);
	aux = mini->envp;
	if (!aux)
	{
		revert_redir(mini, cmd);
		return (1);
	}
	while (aux && *aux)
		printf("%s\n", *aux++);
	revert_redir(mini, cmd);
	return (0);
}
