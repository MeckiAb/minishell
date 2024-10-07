/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labderra <labderra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 12:21:28 by labderra          #+#    #+#             */
/*   Updated: 2024/10/07 17:51:14 by labderra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*join_before_line(char *prefix, char *line)
{
	char	*temp;
	int		i;
	int		j;

	temp = malloc(sizeof(char) * (ft_strlen(prefix) + ft_strlen(line) + 1));
	if (!temp)
		return (NULL);
	i = -1;
	while (prefix[++i])
		temp[i] = prefix[i];
	j = 0;
	while (line[j])
		temp[i++] = line[j++];
	temp[i] = '\0';
	free(prefix);
	return (temp);
}

void	free_list(t_tkn **tkn_list)
{
	t_tkn	*tmp;

	while (tkn_list && *tkn_list)
	{
		tmp = *tkn_list;
		*tkn_list = (*tkn_list)->next;
		free(tmp->tkn);
		free(tmp);
	}
	*tkn_list = NULL;
}

void	free_split(char **str)
{
	int	i;

	i = 0;
	while (str && str[i])
		free(str[i++]);
	free(str);
}

void	free_commands_and_tokens(t_mini *mini)
{
	t_tkn		*tkn;
	t_command	*cmd;

	while (mini->tkn_list)
	{
		tkn = mini->tkn_list;
		mini->tkn_list = mini->tkn_list->next;
		free(tkn->tkn);
		free(tkn);
	}
	while (mini->cmd_list)
	{
		cmd = mini->cmd_list;
		mini->cmd_list = mini->cmd_list->next;
		free(cmd->path);
		free_split(cmd->arg_array);
		free(cmd);
	}
}

static void	*ft_realloc(void *p, int old_size, int new_size)
{
	void	*r;
	
	if (!new_size)
		return (free(p), NULL);
	r = ft_calloc(1, new_size);
	if (p && old_size <= new_size)
		ft_memmove(r, p, old_size);
	else if (p && old_size > new_size)
		ft_memmove(r, p, new_size);
	return (free (p), r);	
}

char	**add_str_to_array(char *str, char **list)
{
	int		size;
	char	**r;
	
	size = 0;
	while (list && list[size])
		size++;
	size += 1;
	r = ft_realloc(list, sizeof(char *) * size, sizeof(char *) * (size + 1));
	r[size - 1] = ft_strdup(str);
	return (r);
}

char	*check_syntax(t_mini *mini)
{
	t_tkn	*p;
	int		prev_tkn;

	if (!mini->tkn_list)
		return (NULL);
	p = mini->tkn_list;
	prev_tkn = 0;
	while (p)
	{
		if (prev_tkn == 0 && p->tkn_type == 0)
			return (p->tkn);
		else if (prev_tkn == 1 && p->tkn_type != 2)
			return (p->tkn);
		prev_tkn = p->tkn_type;
		p = p->next;
	}
	if (prev_tkn == 0)
		return ("EOF");
	return (NULL);
}

/* 
void	del_empty_tokens(t_tkn **tkn_list)
{
	t_tkn	*p;
	t_tkn	*t;

	while (*tkn_list && *(*tkn_list)->tkn == '\0')
	{
		t = *tkn_list;
		*tkn_list = (*tkn_list)->next;
		free(t->tkn);
		free(t);
	}
	p = *tkn_list;
	while (p)
	{
		if (p->next && *p->next->tkn == '\0')
		{
			t = p->next;
			p->next =p->next->next;
			free(t->tkn);
			free(t);
		}
		else
			p = p->next;
	}
}
 */

void	print_errors(char *s1, char *s2, char *s3)
{
	ft_putstr_fd(s1, 2);
	ft_putstr_fd(s2, 2);
	ft_putstr_fd(s3, 2);
}