/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jormoral <jormoral@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 11:49:48 by jormoral          #+#    #+#             */
/*   Updated: 2024/10/10 13:44:02 by jormoral         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int run_cd(t_mini *mini, t_command *cmd)
{
	int	result;
	int	i;
	char *new_old_pwd;
	char *new_pwd;
	char *tilde_path;
	
	
	new_old_pwd = getcwd(NULL, 0);
	i = 0;
	result = 0;
	if(cmd->arg_array && (!cmd->arg_array[1] || cmd->arg_array[1][0] == '~'))
	{
		i = search_env(mini, "HOME=");
		if(cmd->arg_array[1][0] == '~' && cmd->arg_array[1][1])
		{
			tilde_path = ft_strjoin(mini->envp[i] + 5, cmd->arg_array[1] + 1);
			printf("%s\n\n\n", tilde_path);
			if(chdir(tilde_path) == -1)
				perror("HOME NOT SET");
		}
		else if(chdir(mini->envp[i] + 5) == -1)
			perror("HOME NOT SET");
	}
	else if(cmd->arg_array && cmd->arg_array[1][0] == '-')
	{
		i = search_env(mini, "OLDPWD=");
		if(chdir(mini->envp[i] + 7) == -1)
			perror("OLDPWD NOT SET");
	}
	else
	{
		if(chdir(cmd->arg_array[1]) == -1)
			perror("NOT FOUND");
	}
	i = 0;
	i = search_env(mini, "OLDPWD");
	mini->envp[i] = ft_strjoin("OLDPWD=", new_old_pwd);
	new_pwd = getcwd(NULL, 0);
	i = search_env(mini, "PWD");
	mini->envp[i] = ft_strjoin("PWD=", new_pwd);
	mini->envp_dictionary = copy_split_environment(mini->envp);
	return(result); 
}
