/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labderra <labderra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 14:48:11 by labderra          #+#    #+#             */
/*   Updated: 2024/10/18 14:50:11 by labderra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	if (prev_tkn == 0 || prev_tkn == 1)
		return ("EOF");
	return (NULL);
}

int	len_before_equal(char *s)
{
	int	i;

	i = 0;
	while (s && s[i] && s[i] != '=')
		i++;
	return (i);
}

void	print_errors(char *s1, char *s2, char *s3)
{
	ft_putstr_fd(s1, 2);
	ft_putstr_fd(s2, 2);
	ft_putstr_fd(s3, 2);
}
