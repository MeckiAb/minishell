/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jormoral <jormoral@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 11:49:48 by jormoral          #+#    #+#             */
/*   Updated: 2024/10/16 19:25:20 by jormoral         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int run_cd(t_mini *mini, t_command *cmd)
{
	int	i;
	char *new_old_pwd;
	char *new_pwd;
	char *tilde_path;
	
	
	new_old_pwd = getcwd(NULL, 0);
	if((!cmd->arg_array[1] || cmd->arg_array[1][0] == '~'))
	{
		i = search_env(mini, "HOME=");
		if (!mini->envp[i])
		{
			printf("Minishell: cd: HOME not set\n");
			return (1);
		}
		if(cmd->arg_array[1] && cmd->arg_array[1][1])
		{
			tilde_path = ft_strjoin(mini->envp[i] + 5, cmd->arg_array[1] + 1);
			if(chdir(tilde_path) == -1)
			{
				perror("HOME NOT SET");
				free(tilde_path);
				return (1);
			}
		}
		else if(chdir(mini->envp[i] + 5) == -1)
		{
			perror("HOME NOT SET");
			return (1);
		}
	}
	else if(cmd->arg_array[1] && cmd->arg_array[1][0] == '-')
	{
		i = search_env(mini, "OLDPWD=");
		if (!mini->envp[i])
			return (1);
		if(chdir(mini->envp[i] + 7) == -1)
		{
			perror("OLDPWD NOT SET");
			return (1);
		}
	}
	else
	{
		if(chdir(cmd->arg_array[1]) == -1)
		{
			perror("NOT FOUND manolo");
			return (257);
		}
	}
	i = search_env(mini, "OLDPWD");
	if (!mini->envp[i])
		return (1);
	free(mini->envp[i]);
	mini->envp[i] = ft_strjoin("OLDPWD=", new_old_pwd);
	new_pwd = getcwd(NULL, 0);
	i = search_env(mini, "PWD");
	if (!mini->envp[i])
		return (1);
	free(mini->envp[i]);
	mini->envp[i] = ft_strjoin("PWD=", new_pwd);
	free_dictionary(mini->envp_dictionary);
	mini->envp_dictionary = copy_split_environment(mini->envp);
	free(new_old_pwd);
	free(new_pwd);
	return(0); 
}