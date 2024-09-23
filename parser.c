/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labderra <labderra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 10:28:37 by labderra          #+#    #+#             */
/*   Updated: 2024/09/23 13:17:29 by labderra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_command	*new_command(void)
{
	t_command	*cmd;
	
	cmd = malloc(sizeof(t_command));
	if (!cmd)
		exit(EXIT_FAILURE);
	cmd->arg_array = ft_calloc(sizeof(char *), 1);
	if (!cmd->arg_array)
		exit(EXIT_FAILURE);
	cmd->path = NULL;
	cmd->infile = 0;
	cmd->outfile = 1;
	cmd->exit_status = -1;
	cmd->next = NULL;
	return (cmd);
}

static void heredoc(void)
{
	
}

static void	handle_redir(char *redir, char *filename, t_command *cmd)
{
	if (!ft_strncmp(redir, "<<", ft_strlen(redir)))
		heredoc();
	else if (!ft_strncmp(redir, "<", ft_strlen(redir)))
	{
		cmd->infile = open(filename, O_RDONLY);
		if (cmd->infile == -1)
			perror(filename);
		dup2(cmd->infile, STDIN_FILENO);
		close(cmd->infile);
	}
	else if (!ft_strncmp(redir, ">>", ft_strlen(redir)))
	{
		cmd->outfile = open(filename, O_WRONLY | O_APPEND | O_CREAT, 0664);
		if (cmd->outfile == -1)
			perror(filename);
		dup2(cmd->outfile, STDOUT_FILENO);
		close(cmd->outfile);
	}
	else if (!ft_strncmp(redir, ">", ft_strlen(redir)))
	{
		cmd->outfile = open(filename, O_WRONLY | O_TRUNC | O_CREAT, 0664);
		if (cmd->outfile == -1)
			perror(filename);
		dup2(cmd->outfile, STDOUT_FILENO);
		close(cmd->outfile);
	}
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
				handle_redir(p->tkn, p->next->tkn, cmd);
				p = p->next;
			}
			else if (p->tkn_type == 2)
				handle_name(p->tkn, cmd);
			p = p->next;
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