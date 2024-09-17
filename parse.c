/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labderra <labderra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 19:05:04 by labderra          #+#    #+#             */
/*   Updated: 2024/09/17 13:47:19 by labderra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	insert_token(t_mini *mini, char *str, int tkn_type)
{
	t_tkn	*t;
	t_tkn	*p;

	t = malloc(sizeof(t_tkn));
	if (!t)
		return ;
	t->tkn_type = tkn_type;
	t->next = NULL;
	t->tkn = ft_strdup(str);
	if (!t->tkn)
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

static void	insert_control(t_mini *mini, char **str)
{
	if (**str == '<' && *(*str + 1) == '<' && ++*str && ++*str)
		insert_token(mini, "<<", 1);
	else if (**str == '>' && *(*str + 1) == '>' && ++*str && ++*str)
		insert_token(mini, ">>", 1);
	else if (**str == '<' && ++*str)
		insert_token(mini, "<", 1);
	else if (**str == '>' && ++*str)
		insert_token(mini, ">", 1);
	else if (**str == '|' && ++*str)
		insert_token(mini, "|", 0);
}

static int	select_quote(int quote, char c)
{
	if (quote == 0 && c == '\"')
		return (1);
	else if (quote == 0 && c == '\'')
		return (-1);
	else if (quote == 1 && c == '\"')
		return (0);
	else if (quote == 1 && c == '\'')
		return (1);
	else if (quote == -1 && c == '\"')
		return (-1);
	else if (quote == -1 && c == '\'')
		return (0);
	return (0);
}

static void	insert_word(t_mini *mini, char **str)
{
	int		j;
	char	*tmp;
	int		quote;

	j = 0;
	quote = 0;
	tmp = ft_calloc(sizeof(char), 1024);
	while (**str && (quote || !(**str == ' ' || **str == '\t' || **str == '\n'
		|| **str == '<' || **str == '>' || **str == '|')))
	{
		if ((**str == '\'' && quote <= 0) || (**str == '\"' && quote >= 0))
			quote = select_quote(quote, *(*str)++);
		else if (**str == '$' && quote >= 0 && ++*str)
			insert_variable_value(mini, str);
		else
			tmp[j++] = *(*str)++;
	}
	if (!quote)
		insert_token(mini, tmp, 2);
	free(tmp);
}

void	parse_line(t_mini *mini, char *str)
{
	while (str && *str)
	{
		while (*str == ' ' || *str == '\t' || *str == '\n')
			str++;
		if (!*str)
			break ;
		else if (*str == '<' || *str == '>' || *str == '|')
			insert_control(mini, &str);
		else
			insert_word(mini, &str);
	}
	del_empty_tokens(&(mini->tkn_list));
}
