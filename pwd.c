/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labderra <labderra@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 00:10:16 by labderra          #+#    #+#             */
/*   Updated: 2024/10/24 00:20:19 by labderra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	run_pwd(t_mini *mini, t_command *cmd)
{
	char	*pwd;

	apply_redir(cmd);
	pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		revert_redir(mini, cmd);
		return (1);
	}
	else
	{
		printf("%s\n", pwd);
		free (pwd);
		revert_redir(mini, cmd);
		return (0);
	}
}
