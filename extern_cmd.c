/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extern_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labderra <labderra@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 21:34:53 by labderra          #+#    #+#             */
/*   Updated: 2024/10/23 21:36:43 by labderra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**get_full_path(char **envp)
{
	while (envp && *envp && ft_strncmp(*envp, "PATH=", 5))
		envp++;
	if (!envp || !*envp)
		return (ft_split("", ' '));
	return (ft_split(*envp + 5, ':'));
}

static void	run_child_process(t_mini *mini, t_command *cmd,
		char *path_cmd, int fd[2])
{
	int	pid;

	pid = fork();
	if (!pid)
	{
		apply_redir(cmd);
		if (cmd->infile == -1 || cmd->outfile == -1)
			exit(1);
		close(fd[0]);
		execve(path_cmd, cmd->arg_array, mini->envp);
		revert_redir(mini, cmd);
		perror(cmd->arg_array[0]);
		exit(0);
	}
	else
		cmd->pid = pid;
}

void	run_execve_command(t_mini *mini, t_command *cmd, int fd[2])
{
	char	**aux;
	char	*path_cmd;
	int		i;

	aux = get_full_path(mini->envp);
	i = 0;
	path_cmd = ft_strdup(cmd->arg_array[0]);
	while (aux && aux[i] && path_cmd && access(path_cmd, X_OK) != 0)
	{
		free(path_cmd);
		path_cmd = triple_strjoin(aux[i++], "/", cmd->arg_array[0]);
	}
	if (access(path_cmd, X_OK) == 0)
		run_child_process(mini, cmd, path_cmd, fd);
	else
	{
		if (cmd->arg_array && cmd->arg_array[0] && !aux[i])
		{
			cmd->exit_status = 127 << 8;
			print_errors(cmd->arg_array[0], ": command not found", "\n");
		}
	}
	free_split(aux);
	free(path_cmd);
	revert_redir(mini, cmd);
}
