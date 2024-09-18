/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labderra <labderra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 12:21:28 by labderra          #+#    #+#             */
/*   Updated: 2024/09/18 11:47:01 by labderra         ###   ########.fr       */
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