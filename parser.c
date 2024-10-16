/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labderra <labderra@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 10:28:37 by labderra          #+#    #+#             */
/*   Updated: 2024/10/16 05:54:15 by labderra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_command	*new_command(void)
{
	t_command	*cmd;
	
	cmd = malloc(sizeof(t_command));
	if (!cmd)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	cmd->arg_array = ft_calloc(sizeof(char *), 1);
	if (!cmd->arg_array)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	cmd->path = NULL;
	cmd->infile = -1;
	cmd->outfile = -1;
	cmd->pid = -1;
	cmd->exit_status = -1;
	cmd->next = NULL;
	return (cmd);
}

static char	*expand_heredoc_dollar(t_mini *mini, char **str)
{
	int		i;
	char	*tmp;
	char	*backup;

	i = 0;
	tmp = ft_calloc(sizeof(char), 4096);
	if (!tmp)
		return (NULL);
	backup = *str;
	while (*str && **str)
	{
		if (**str == '$' && (ft_isalnum(*(*str + 1)) || *(*str + 1) == '_'))
		{
			++*str;
			insert_variable_value(mini, str);
		}
		else
			tmp[i++] = *(*str)++;
	}
	free(backup);
	return(tmp);
}

static int heredoc(t_mini *mini, char *lmt, int xpand)
{
	char	*aux_str;
	int		fd[2];
	int		size;

	size = ft_strlen(lmt);
	if (pipe(fd) == -1)
		return (-1);
	while(1)
	{
		aux_str = readline("heredoc>");
		if (!aux_str)
			break ;
		if (!ft_strncmp(aux_str, lmt, size + 1))
			break ;
		if (xpand)
			aux_str = expand_heredoc_dollar(mini, &aux_str);
		write(fd[1], aux_str, ft_strlen(aux_str));
		write(fd[1], "\n", 1);
		free(aux_str);
	}
	free(aux_str);
	close(fd[1]);
	return (fd[0]);
}

int	heredoc_launcher(t_mini *mini, char *lmt, int xpand)
{
	int	pid;
	int	status;
	int *result;

	status = 0;
	result = malloc(sizeof(int));
	*result = -1;
	pid = fork();
	if (!pid)
	{
		signal(SIGINT, SIG_DFL);
		*result = heredoc(mini, lmt, xpand);
		exit(0);
	}
	else
	{
		signal(SIGINT, handle_sigint_fork);
		waitpid(pid, &status, 0);
	}
	return (*result);
}

static void	handle_redir(t_mini *mini, t_command *cmd, char *redir, char *filename)
{
	int	file_fd;
	
	if (!ft_strncmp(redir, "<<", 3))
		file_fd = heredoc_launcher(mini, filename, 0);
	else if (!ft_strncmp(redir, "<$", 3))
		file_fd = heredoc_launcher(mini, filename, 1);
	else if (!ft_strncmp(redir, "<", 2))
		file_fd = open(filename, O_RDONLY);
	else if (!ft_strncmp(redir, ">>", 3))
		file_fd = open(filename, O_WRONLY | O_APPEND | O_CREAT, 0664);
	else
		file_fd = open(filename, O_WRONLY | O_TRUNC | O_CREAT, 0664);
	if (file_fd == -1 && !global_signal)
		perror("file error");
	if (*redir == '<')
		cmd->infile = file_fd;
	else
		cmd->outfile = file_fd;
}

static void	handle_name(char *name, t_command *cmd)
{
	cmd->arg_array =  add_str_to_array(name, cmd->arg_array);
}

static void	insert_command(t_mini *mini, t_command *cmd)
{
	t_command	*p;

	if (!mini->cmd_list)
		mini->cmd_list = cmd;
	else
	{
		p = mini->cmd_list;
		while (p->next)
			p = p->next;
		p->next = cmd;
	}	
}

void	parser(t_mini *mini)
{
	t_command	*cmd;
	t_tkn		*p;

	p = mini->tkn_list;
	while (p)
	{
		cmd = new_command();
		while (p && p->tkn_type != 0)
		{
			if (p->tkn_type == 1)
			{
				handle_redir(mini, cmd, p->tkn, p->next->tkn);
				p = p->next;
			}
			else if (p->tkn_type == 2)
				handle_name(p->tkn, cmd);
			p = p->next;
			if (global_signal)
			{
				mini->status = 130;
				free_commands_and_tokens(mini);
				global_signal = 0;
				return ;
			}
		}
		if (p && p->tkn_type == 0)
		{
			insert_command(mini, cmd);
			cmd = new_command();
			p = p->next;
		}
		else
			insert_command(mini, cmd);
	}
}