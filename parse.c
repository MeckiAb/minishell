/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labderra <labderra@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 19:05:04 by labderra          #+#    #+#             */
/*   Updated: 2024/09/16 00:34:44 by labderra         ###   ########.fr       */
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

/* static void	insert_control(t_mini *mini, char *str, int *i)
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
} */
/* 
void	insert_word(t_mini *mini, char *str, int *i)
{
	int		j;
	char	*tmp;
	int		toogle_expand;
	int		quote;

	if (!str)
		exit(EXIT_FAILURE);
	j = 0;
	toogle_expand = 1;
	quote = -1;
	tmp = ft_calloc(sizeof(char), (ft_strlen(str) + 1));
	while (str[*i] && ((!is_sp(str[*i]) && !is_ctrl(str[*i])) || quote == 1))
	{
		if ((str[*i] == '\'' && !ft_strchr(&str[*i + 1], '\''))
			|| (str[*i] == '\"' && !ft_strchr(&str[*i + 1], '\"')))
			exit(EXIT_FAILURE);
		else if (str[*i] == '\'' && ++*i)
		{
			toogle_expand *= -1;
			quote *= -1;
		}
		else if (str[j] == '\"' && ++*i)
			quote *= -1;
		else if (str[*i] == '$' && toogle_expand == 1 && ++*i)
			insert_env_item(mini, str, i);
		else
			tmp[j++] = str[*i++];
	}
	insert_token(mini, tmp, 2);
	free(tmp);
} */
/* 
void	parse_line(t_mini *mini, char *str)
{
	int	i;

	i = 0;
	while (str && str[i])
	{
		while (is_sp(str[i]))
			i += 1;
		if (is_ctrl(str[i]))
			insert_control(mini, str, &i);
		else
			insert_word(mini, str, &i);
	}
} */

static void	insert_control(t_mini *mini, char **str)
{
	if (**str == '<' && *(*str + 1) == '<' && *str++ && ++*str)
		insert_token(mini, "<<", 1);
	else if (**str == '<' && *(*str + 1) == '<' && *str++ && ++*str)
		insert_token(mini, ">>", 1);
	else if (**str == '<' && ++*str)
		insert_token(mini, "<", 1);
	else if (**str == '>' && ++*str)
		insert_token(mini, ">", 1);
	else if (**str == '|' && ++*str)
		insert_token(mini, "|", 0);
}

static int	select_expand_case(int quote, char c)
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
		if (**str == '\'' || **str == '\"')
			quote = select_quote(quote, *(*str)++);
		else if (**str == '$' && quote >= 0 && ++*str)
			insert_env_item(mini, str, i);
		else
			tmp[j++] = *(*str)++;
	}
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
}


/* 
void	parse_line(t_mini *mini, char *str)
{
	int		quote;
	char	*chunk;
	char	*tmp;
	
	quote = 0;
	chunk = ft_calloc(sizeof(char), 1024);
	tmp = chunk;
	while (str && *str)
	{
		if (!quote && (*str == ' ' || *str == '\t' || *str == '\n'))
			str = str++;
		else if (quote == 0 && ((*str == '<' && *(str + 1) == '<')
			|| (*str == '>' && *(str + 1) == '>')) && str++)
			insert_token(mini, *str++, 2);
		else if (quote == 0 && (*str == '<' || *str == '>'))
			insert_token(mini, *str++, 1);
		else if (quote == 0 && *str == '|')
			insert_token(mini, *str++, 0);
		else if (*str == '\'' || *str == '\"')
			toogle_quote(*str++, &quote);
		else if (quote >= 0 && *str == '$' && str++)
			str = join_line(insert_env_item(mini, str), str);
		else
			while (*str && !quote || (*str == ' ' || *str == '\t' || *str == '\n' ))
			
	}
} */