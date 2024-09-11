/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labderra <labderra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 19:05:04 by labderra          #+#    #+#             */
/*   Updated: 2024/09/11 14:10:23 by labderra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_space(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

int	is_control(char c)
{
	return (c == '<' || c == '>' || c == '|');
}

void	insert_token(t_mini *mini, char *str, int tkn_type)
{
	
}

void	insert_control(t_mini *mini, char *str, int *i)
{
	if (str[*i] == '<' && str[*i + 1] == '<' && (*i += 2))
		insert_token(mini, "<<", 1);
	else if (str[*i] == '>' && str[*i + 1] == '>' && (*i += 2))
		insert_token(mini, ">>", 1);
	else if (str[*i] == '<' && (*i += 1))
		insert_token(mini, "<", 1);
	else if (str[*i] == '>' && (*i += 1))
		insert_token(mini, ">", 1);
	else if (str[*i] == '|' && (*i += 1))
		insert_token(mini, "|", 0);
}

void	insert_word(t_mini *mini, char *str, int *i)
{
	int		j;
	char	*tmp;
	int		size;

	if (!str)
		return ;
	j = 0;
	tmp = ft_calloc(sizeof(char), (ft_strlen(str) + 1));
	while (!is_space(str[*i]) && !is_control(str[*i]))
	{
		if ((str[*i] == '\'' && !ft_strchr(&str[*i + 1], '\''))
				||(str[*i] == '\"' && !ft_strchr(&str[*i + 1], '\"')))
			quote_error();
		else if (str[*i] == '\'')
		{
			size = ft_strchr(&str[*i + 1], '\'') - &str[*i + 1];
			ft_memcpy(&tmp[j], &str[*i], size);
			j += size;
			*i = *i + size + 2;
		}
		else if (str[j] == '\"')
		{
			size = ft_strchr(&str[*i + 1], '\"') - &str[*i + 1];
			ft_memcpy(&tmp[j], &str[*i], size);
			j += size;
			*i = *i + size + 2;
			expand();
		}
		else if (str[*i] == '$')
			expand(tmp, j, );
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