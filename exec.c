/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labderra <labderra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 11:07:06 by labderra          #+#    #+#             */
/*   Updated: 2024/09/26 20:47:23 by labderra         ###   ########.fr       */
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

/* 
void	run_command(t_mini *mini, t_command *cmd)
{
	char *s;
	
	s = ft_strdup(cmd->arg_array[0]);
	if (!ft_strncmp(s, "echo", ft_strlen(s)))
		run_echo(cmd);
	else if (!ft_strncmp(s, "cd", ft_strlen(s)))
		run_cd(cmd);
	else if (!ft_strncmp(s, "pwd", ft_strlen(s)))
		run_pwd();
	else if (!ft_strncmp(s, "export", ft_strlen(s)))
		run_export(cmd);
	else if (!ft_strncmp(s, "unset", ft_strlen(s)))
		run_unset(cmd);
	else if (!ft_strncmp(s, "env", ft_strlen(s)))
		run_env(cmd);
	else if (!ft_strncmp(s, "exit", ft_strlen(s)))
		run_exit(cmd);
	else
		run_command(cmd, mini);
}
 */

void	run_execve_command(t_mini *mini, t_command *cmd)
{
	char	**aux;
	char	*path_cmd;
	int		i;
	
	i = 0;
	aux = mini->path;
	path_cmd = triple_strjoin(".", "/", cmd->arg_array[0]);
	if(path_cmd && access(path_cmd, X_OK) == 0)
		execve(path_cmd, cmd->arg_array, mini->envp);
	free(path_cmd);
	while(aux[i])
	{
		path_cmd = triple_strjoin(aux[i], "/", cmd->arg_array[0]);
		if(path_cmd && access(path_cmd, X_OK) == 0)
			execve(path_cmd, cmd->arg_array, mini->envp);
		i++;
		free(path_cmd);
	}
	if (cmd->arg_array && cmd->arg_array[0])
		perror("Command not found");
}


void	exec_line(t_mini *mini)
{
	int			fd[2];
	int			pipe_in;
	int			pid;
	t_command	*cmd;

	cmd = mini->cmd_list;
	pipe_in = STDIN_FILENO;
	fd[0] = STDIN_FILENO;
	fd[1] = STDOUT_FILENO;
	while (cmd)
	{
		if (cmd->next && pipe(fd) == -1)
			perror("pipe");
		pid = fork();
		if (!pid)
		{
			close(fd[0]);
			dup2(pipe_in, STDIN_FILENO);
			if (pipe_in != 0)
				close(pipe_in);
			dup2(fd[1], STDOUT_FILENO);
			if (fd[1] != 1)
				close(fd[1]);
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
			//run_command(mini, cmd);
			run_execve_command(mini, cmd);
		}
		if (pipe_in != 0)
			close(pipe_in);
		pipe_in = dup(fd[0]);
		close(fd[0]);
		close(fd[1]);
		fd[1] = STDOUT_FILENO;
		cmd->pid = pid;
		cmd = cmd->next;
	}
	if(pipe_in != 0)
		close(pipe_in);
	cmd = mini->cmd_list;
	while (cmd)
	{
		waitpid(cmd->pid, &(cmd->exit_status), 0);
		cmd = cmd->next;
	}
}
