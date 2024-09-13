/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labderra <labderra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 19:05:04 by labderra          #+#    #+#             */
/*   Updated: 2024/09/13 12:04:31 by labderra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	insert_token(t_mini *mini, char *str, int tkn_type)
{
	t_tkn	*t;
	t_tkn	*p;

	t = malloc(sizeof(t_tkn));
	if (!t)
		return ;
	t->tkn_type = tkn_type;
	t->next = NULL;
	t->tkn = ft_strdup(str);
	if (!t)
		return ;
	if (!mini->tkn_list)
		mini->tkn_list = t;
	else
	{
		p = mini->tkn_list;
		while (p->next)
			p = p->next;
		p->next = t;
	}
}

static void	insert_control(t_mini *mini, char *str, int *i)
{
	if (str[*i] == '<' && str[*i + 1] == '<' && ++*i && ++*i)
		insert_token(mini, "<<", 1);
	else if (str[*i] == '>' && str[*i + 1] == '>' && ++*i && ++*i)
		insert_token(mini, ">>", 1);
	else if (str[*i] == '<' && ++*i)
		insert_token(mini, "<", 1);
	else if (str[*i] == '>' && ++*i)
		insert_token(mini, ">", 1);
	else if (str[*i] == '|' && ++*i)
		insert_token(mini, "|", 0);
}

void	insert_word(t_mini *mini, char *str, int *i)
{
	int		j;
	char	*tmp;
	int		toogle_expand;
	int		toogle_quote;

	j = 0;
	toogle_expand = 1;   // ESTA MIERDA NO FUNCIONARA, PROBAR CON BOOLEANOS O ALGO ASI
	toogle_quote = -1;
	tmp = ft_calloc(sizeof(char), (ft_strlen(str) + 1));
	while (str[*i] && !is_space(str[*i]) && !is_control(str[*i]) || toogle_quote)
	{
		if ((str[*i] == '\'' && !ft_strchr(&str[*i + 1], '\''))
			|| (str[*i] == '\"' && !ft_strchr(&str[*i + 1], '\"')))
			exit(EXIT_FAILURE);
		else if (str[*i] == '\'' && ++*i)
		{
			toogle_expand *= -1;
			toogle_quote *= -1;
		}
		else if (str[j] == '\"' && ++*i)
			toogle_quote *= -1;
		else if (str[*i] == '$' && toogle_expand && ++*i)
			insert_env_item(mini, str, i);
		else
			tmp[j++] = str[*i++];
	}
	insert_token(mini, tmp, 2);
	free(tmp);
}

void	parse_line(t_mini *mini, char *str)
{
	int	i;

	i = 0;
	while (str && str[i])
	{
		while (is_space(str[i]))
			i += 1;
		if (is_control(str[i]))
			insert_control(mini, str, &i);
		else
			insert_word(mini, str, &i);
	}
	return (0);
}
