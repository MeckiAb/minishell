/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labderra <labderra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 10:28:37 by labderra          #+#    #+#             */
/*   Updated: 2024/10/23 14:23:54 by labderra         ###   ########.fr       */
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
	cmd->infile = -2;
	cmd->outfile = -2;
	cmd->pid = -1;
	cmd->exit_status = -1;
	cmd->next = NULL;
	return (cmd);
}

static void	handle_redir(t_mini *mini, t_command *cmd, char *redir, char *fl)
{
	int	file_fd;

	if (!ft_strncmp(redir, "<<", 3))
		file_fd = heredoc_launcher(mini, fl, 0);
	else if (!ft_strncmp(redir, "<$", 3))
		file_fd = heredoc_launcher(mini, fl, 1);
	else if (!ft_strncmp(redir, "<", 2))
		file_fd = open(fl, O_RDONLY);
	else if (!ft_strncmp(redir, ">>", 3))
		file_fd = open(fl, O_WRONLY | O_APPEND | O_CREAT, 0664);
	else
		file_fd = open(fl, O_WRONLY | O_TRUNC | O_CREAT, 0664);
	if (file_fd == -1 && !g_signal)
	{
		print_errors("MiniShell: ", fl, ": ");
		print_errors(strerror(errno), "\n", "");
	}
	if (*redir == '<')
		cmd->infile = file_fd;
	else
		cmd->outfile = file_fd;
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

t_tkn	*process_token(t_mini *mini, t_command *cmd, t_tkn *p)
{
	if (p->tkn_type == 1)
	{
		handle_redir(mini, cmd, p->tkn, p->next->tkn);
		p = p->next;
	}
	else if (p->tkn_type == 2)
		cmd->arg_array = add_str_to_array(p->tkn, cmd->arg_array);
	p = p->next;
	return (p);
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
			p = process_token(mini, cmd, p);
		if (g_signal)
		{
			mini->status = 130;
			free_commands_and_tokens(mini);
			free_split(cmd->arg_array);
			free(cmd);
			g_signal = 0;
			break ;
		}
		insert_command(mini, cmd);
		if (p && p->tkn_type == 0)
			p = p->next;
	}
}
