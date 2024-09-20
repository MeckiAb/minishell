/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labderra <labderra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 10:28:37 by labderra          #+#    #+#             */
/*   Updated: 2024/09/20 13:13:10 by labderra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_command	*new_command()
{
	t_command	*cmd;
	
	cmd = malloc(sizeof(t_command));
	if (!cmd)
		exit(EXIT_FAILURE);
	cmd->arg_array = malloc(sizeof(char *));
	if (!cmd->arg_array)
		exit(EXIT_FAILURE);
	cmd->arg_array[0] = NULL;
	cmd->path = NULL;
	cmd->infile = 0;
	cmd->outfile = 1;
	cmd->exit_status = -1;
	cmd->next = NULL;
	return (cmd);
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
	}
	else if (!ft_strncmp(redir, ">>", ft_strlen(redir)))
	{
		cmd->outfile = open(filename, O_WRONLY | O_APPEND | O_CREAT, 664);
		if (cmd->outfile == -1)
			perror(filename);
	}
	else if (!ft_strncmp(redir, ">", ft_strlen(redir)))
	{
		cmd->outfile = open(filename, O_WRONLY | O_TRUNC | O_CREAT, 664);
		if (cmd->outfile == -1)
			perror(filename);
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

	cmd = new_command();
	p = mini->tkn_list;
	while (p)
	{
		if (p->tkn_type == 1)
		{
			handle_redir(p->tkn, p->next->tkn, cmd);
			p = p->next->next;
		}
		else if (p->tkn_type == 2)
		{
			handle_name(p->tkn, cmd);
			p = p->next;
		}
		else
		{
			insert_command(mini, cmd);
			cmd = new_command();
			p = p->next;
		}
	}
	insert_command(mini, cmd);
}