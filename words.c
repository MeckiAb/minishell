/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   words.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labderra <labderra@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 14:27:57 by labderra          #+#    #+#             */
/*   Updated: 2024/10/25 08:05:19 by labderra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static int	expand_word(t_mini *mini, char **str, char **tmp)
{
	int		j;
	char	*value;
	char	*aux;

	value = insert_variable_value(mini, str);
	j = ft_strlen(value);
	aux = ft_strjoin(*tmp, value);
	free(*tmp);
	free(value);
	*tmp = ft_realloc(aux, ft_strlen(aux), 4096);
	return (j);
}

void	insert_word(t_mini *mini, char **str)
{
	int		j;
	char	*tmp;
	int		quote;

	j = 0;
	quote = 0;
	tmp = ft_calloc(sizeof(char), 4096);
	while (**str && (quote || !(**str == ' ' || **str == '\t' || **str == '\n'
				|| **str == '<' || **str == '>' || **str == '|')))
	{
		if ((**str == '\'' && quote <= 0) || (**str == '\"' && quote >= 0))
			quote = select_quote(quote, *(*str)++);
		else if (**str == '$' && quote >= 0 && ((*(*str + 1) == '?'
					|| (ft_isalnum(*(*str + 1))
						|| *(*str + 1) == '_'))) && ++*str)
			j += expand_word(mini, str, &tmp);
		else
			tmp[j++] = *(*str)++;
	}
	if (!quote)
		insert_token(mini, tmp, 2);
	free(tmp);
}
