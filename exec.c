/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labderra <labderra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 11:07:06 by labderra          #+#    #+#             */
/*   Updated: 2024/10/23 13:50:18 by labderra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	run_child_process(t_mini *mini, t_command *cmd,
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

void	run_command(t_mini *mini, t_command *cmd, int fd[2])
{
	if (!cmd->arg_array[0])
		cmd->arg_array[0] = ft_strdup("");
	if (!ft_strncmp(cmd->arg_array[0], "echo", 5))
		cmd->exit_status = run_echo(mini, cmd);
	else if (!ft_strncmp(cmd->arg_array[0], "cd", 3))
		cmd->exit_status = run_cd(mini, cmd);
	else if (!ft_strncmp(cmd->arg_array[0], "pwd", 4))
		cmd->exit_status = run_pwd(mini, cmd);
	else if (!ft_strncmp(cmd->arg_array[0], "export", 7))
		cmd->exit_status = run_export(mini, cmd);
	else if (!ft_strncmp(cmd->arg_array[0], "unset", 6))
		cmd->exit_status = run_unset(mini, cmd);
	else if (!ft_strncmp(cmd->arg_array[0], "env", 4))
		cmd->exit_status = run_env(mini, cmd);
	else if (!ft_strncmp(cmd->arg_array[0], "exit", 5))
		cmd->exit_status = run_exit(mini, cmd);
	else
		run_execve_command(mini, cmd, fd);
}

static void	wait_process(t_mini *mini)
{
	t_command	*cmd;

	cmd = mini->cmd_list;
	while (cmd)
	{
		if (cmd->pid != -1)
		{
			signal(SIGINT, handle_sigint_fork);
			signal(SIGQUIT, handle_sigquit_fork);
			waitpid(cmd->pid, &(cmd->exit_status), 0);
			mini->status = WEXITSTATUS(cmd->exit_status);
		}
		else
			mini->status = WEXITSTATUS(cmd->exit_status);
		if (g_signal)
			mini->status = 128 + g_signal;
		cmd = cmd->next;
	}
	g_signal = 0;
}

void	exec_line(t_mini *mini)
{
	int			fd[2];
	t_command	*cmd;

	cmd = mini->cmd_list;
	while (cmd && cmd->next)
	{
		if (pipe(fd) == -1)
			perror("pipe");
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
		run_command(mini, cmd, fd);
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		cmd = cmd->next;
	}
	if (cmd)
	{
		dup2(mini->mini_out, STDOUT_FILENO);
		run_command(mini, cmd, fd);
		dup2(mini->mini_in, STDIN_FILENO);
	}
	wait_process(mini);
}
