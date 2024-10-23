/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labderra <labderra@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 11:49:48 by jormoral          #+#    #+#             */
/*   Updated: 2024/10/23 23:25:54 by labderra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	cd_home(t_mini *mini, t_command *cmd)
{
	int		i;
	char	*tilde_path;

	i = search_env(mini, "HOME=");
	if (!mini->envp[i])
	{
		printf("MiniShell: cd: %s\n", strerror(errno));
		return (257);
	}
	if (cmd->arg_array[1] && cmd->arg_array[1][1])
	{
		tilde_path = ft_strjoin(mini->envp[i] + 5, cmd->arg_array[1] + 1);
		if (chdir(tilde_path) == -1)
		{
			printf("MiniShell: cd: %s\n", strerror(errno));
			free(tilde_path);
			return (257);
		}
	}
	else if (chdir(mini->envp[i] + 5) == -1)
	{
		printf("MiniShell: cd: %s\n", strerror(errno));
		return (257);
	}
	return (0);
}

static int	cd_back(t_mini *mini)
{
	int	i;

	i = search_env(mini, "OLDPWD=");
	if (chdir(mini->envp[i] + 7) == -1)
	{
		printf("MiniShell: cd: %s\n", strerror(errno));
		return (257);
	}
	return (0);
}

static int	cd_directory(t_command *cmd)
{
	if (chdir(cmd->arg_array[1]) == -1)
	{
		printf("MiniShell: cd: %s: %s\n", cmd->arg_array[1], strerror(errno));
		return (257);
	}
	return (0);
}

void	fix_pwd(t_mini *mini, char *new_pwd, char *new_old_pwd)
{
	t_command	*cmd;
	char		*cmd_text;

	cmd = ft_calloc(sizeof(t_command), 1);
	cmd_text = ft_strjoin("export OLDPWD=", new_old_pwd);
	cmd->arg_array = ft_split(cmd_text, ' ');
	run_export(mini, cmd);
	free(cmd_text);
	free_split(cmd->arg_array);
	cmd_text = ft_strjoin("export PWD=", new_pwd);
	cmd->arg_array = ft_split(cmd_text, ' ');
	run_export(mini, cmd);
	free(cmd_text);
	free_split(cmd->arg_array);
	free(cmd);
	free_dictionary(mini->envp_dictionary);
	mini->envp_dictionary = copy_split_environment(mini->envp);
}

int	run_cd(t_mini *mini, t_command *cmd)
{
	int		exit_value;
	char	*new_old_pwd;
	char	*new_pwd;

	new_old_pwd = getcwd(NULL, 0);
	if ((!cmd->arg_array[1] || cmd->arg_array[1][0] == '~'))
		exit_value = cd_home(mini, cmd);
	else if (cmd->arg_array[1] && cmd->arg_array[1][0] == '-')
		exit_value = cd_back(mini);
	else
		exit_value = cd_directory(cmd);
	new_pwd = getcwd(NULL, 0);
	fix_pwd(mini, new_pwd, new_old_pwd);
	free(new_old_pwd);
	free(new_pwd);
	return (exit_value);
}
