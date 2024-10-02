/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labderra <labderra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 11:42:34 by jormoral          #+#    #+#             */
/*   Updated: 2024/10/02 11:39:29 by labderra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	run_pwd(t_mini *mini, t_command *cmd)
{
	char	*pwd;

	apply_redir(cmd);
	pwd = getcwd(NULL, 0);
	if(!pwd)
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

int run_echo(t_mini *mini, t_command *cmd)
{
	int nflag;
	int i;

	apply_redir(cmd);
	i = 0;
	nflag = 0;
	if (cmd->arg_array[1] && cmd->arg_array[1][i++] == '-')
	{
		while (cmd->arg_array[1][i] == 'n')
			i += 1;
		nflag = cmd->arg_array[1][i] == '\0';
	}
	i = 1;
	while(cmd->arg_array[i + nflag])
	{
		printf("%s", cmd->arg_array[i + nflag]);
		if(cmd->arg_array[++i + nflag])
			printf(" ");
	}
	if(nflag == 0)
		printf("\n");
	revert_redir(mini, cmd);
	return(0);
}

int	run_export(t_mini *mini, t_command *cmd)
{
	int		i;
	char	***env;
	int		c;
	char	**temp;

	apply_redir(cmd);
	env =  mini->envp_dictionary;
	i = 0;
	while(env[i] && env[i + 1])
	{
		c = 0;
		while(env[i][0][c] == env[i + 1][0][c])
			c++;
		if(env[i][0][c] > env[i + 1][0][c])
		{
			temp = env[i];
			env[i]= env[i + 1];
			env[i + 1] = temp;
			i = 0;
		}
		else
			i++;
	}
	i = 0; 
	while(env[i])
	{
		printf("declare -x %s=\"%s\"\n", env[i][0], env[i][1]);	
		i++;
	}
	revert_redir(mini, cmd);
	return (0);
}
