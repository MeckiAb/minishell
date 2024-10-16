/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labderra <labderra@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 11:07:06 by labderra          #+#    #+#             */
/*   Updated: 2024/10/15 23:46:29 by labderra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*triple_strjoin(char const *s1, char const *s2, char const *s3)
{
	size_t	i;
	char	*p;

	i = 0;
	p = (char *)malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2)
				+ ft_strlen(s3) + 1));
	if (!p)
		return (NULL);
	while (*s1)
		p[i++] = *s1++;
	while (*s2)
		p[i++] = *s2++;
	while (*s3)
		p[i++] = *s3++;
	p[i] = '\0';
	return (p);
}

void	apply_redir(t_command *cmd)
{
	if (cmd->infile != -1)
	{
		dup2(cmd->infile, STDIN_FILENO);
		close(cmd->infile);
	}
	if (cmd->outfile != -1)
	{
		dup2(cmd->outfile, STDOUT_FILENO);
		close(cmd->outfile);
	}
}

void	revert_redir(t_mini *mini, t_command *cmd)
{
	if (cmd->infile != -1)
		dup2(mini->mini_in, STDIN_FILENO);
	if (cmd->outfile != -1)
		dup2(mini->mini_out, STDOUT_FILENO);
	if (!access(".heredoctmp", F_OK))
		unlink(".heredoctmp");
}

/* void	handle_sigint_execve(int sig)
{
	(void)sig;
	printf("\n");
	signal(SIGINT, handle_sigint_main);
}
 */
void	run_execve_command(t_mini *mini, t_command *cmd)
{
	char	**aux;
	char	*path_cmd;
	int		pid;
	int		i;

	aux = get_full_path(mini->envp);
	i = 0;
	path_cmd = ft_strdup(cmd->arg_array[0]);
	while(aux && aux[i] && path_cmd && access(path_cmd, X_OK) != 0)
	{
		free(path_cmd);
		path_cmd = triple_strjoin(aux[i++], "/", cmd->arg_array[0]);
	}
	if (access(path_cmd, X_OK) == 0)
	{
		pid = fork();
		if (!pid)
		{
			apply_redir(cmd);
			execve(path_cmd, cmd->arg_array, mini->envp);
			perror("execve");
			exit(0);
		}
		else
			cmd->pid = pid;
	}
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

void	run_command(t_mini *mini, t_command *cmd)
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
		run_execve_command(mini, cmd);
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
			waitpid(cmd->pid, &(cmd->exit_status), 0);
		}
		if (global_signal)
		{
			mini->status = 130;
			global_signal = 0;
		}
		else
			mini->status = WEXITSTATUS(cmd->exit_status);
		cmd = cmd->next;
	}
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
		run_command(mini, cmd);
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		cmd = cmd->next;
	}
	if (cmd)
	{
		dup2(mini->mini_out, STDOUT_FILENO);
		run_command(mini, cmd);
		dup2(mini->mini_in, STDIN_FILENO);
	}
	wait_process(mini);
}



