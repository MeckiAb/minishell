/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labderra <labderra@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 11:07:06 by labderra          #+#    #+#             */
/*   Updated: 2024/10/23 21:37:08 by labderra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	run_command(t_mini *mini, t_command *cmd, int fd[2])
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
