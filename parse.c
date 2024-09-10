/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labderra <labderra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 19:05:04 by labderra          #+#    #+#             */
/*   Updated: 2024/09/10 19:36:06 by labderra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	insert_token(t_mini *mini, char *str, int tkn_type)
{
	
}

void	insert_control(t_mini *mini, char *str, int *i)
{
	if (str[*i] == '<' && str[*i + 1] == '<')
		insert_token(mini, "<<", 1);
	else if (str[*i] == '>' && str[*i + 1] == '>')
		insert_token(mini, ">>", 1);
	else if (str[*i] == '<')
		insert_token(mini, "<", 1);
	else if (str[*i] == '>')
		insert_token(mini, ">", 1);
	else if (str[*i] == '|')
		insert_token(mini, "|", 0);
}

void	insert_word(t_mini *mini, char *str, int *i)
{
	
}

void	parse(t_mini *mini, char *str)
{
	int	i;
	
	while (str && *str)
	{
		while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n')
			i += 1;
		if (str[i] == '<' || str[i] == '>' || str[i] == '|')
			insert_control(mini, str, &i);
		else 
			insert_word(mini, str, &i);
	}
	free(str);
	return (0);
}