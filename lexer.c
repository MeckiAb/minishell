/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labderra <labderra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 19:05:04 by labderra          #+#    #+#             */
/*   Updated: 2024/10/23 14:29:47 by labderra         ###   ########.fr       */
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
	int	i;

	if (**str == '<' && *(*str + 1) == '<' && ++*str && ++*str)
	{
		i = 0;
		while ((*str)[i] == ' ' || (*str)[i] == '\t' || (*str)[i] == '\n')
			i++;
		if ((*str)[i] == '\'' || (*str)[i] == '\"')
			insert_token(mini, "<<", 1);
		else
			insert_token(mini, "<$", 1);
	}
	else if (**str == '>' && *(*str + 1) == '>' && ++*str && ++*str)
		insert_token(mini, ">>", 1);
	else if (**str == '<' && ++*str)
		insert_token(mini, "<", 1);
	else if (**str == '>' && ++*str)
		insert_token(mini, ">", 1);
	else if (**str == '|' && ++*str)
		insert_token(mini, "|", 0);
}

void	lexer(t_mini *mini, char *str)
{
	char	*tkn;

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
	tkn = check_syntax(mini);
	if (tkn)
	{
		printf("syntax error near unexpected token `%s'\n", tkn);
		free_list(&(mini->tkn_list));
	}
}
