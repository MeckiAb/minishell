/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labderra <labderra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 11:11:44 by labderra          #+#    #+#             */
/*   Updated: 2024/09/16 13:53:28 by labderra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**get_full_path(char **envp)
{
	while (envp && *envp && ft_strncmp(*envp, "PATH=", 5))
		envp++;
	if (!envp || !*envp)
		return (NULL);
	return (ft_split(*envp + 5, ':'));
}

static char	*get_env_item(char **envp, char *item)
{
	if (item && !*item)
		return (item);
	while (envp && *envp && ft_strncmp(*envp, item, ft_strlen(item)))
		envp++;
	if (!envp || !*envp)
		return (ft_strdup(""));
	return (ft_strdup(*envp + ft_strlen(item) + 1));
}

void	insert_variable_value(t_mini *mini, char **str)
{
	char	*identifier;
	char	*value;
	int		j;

	if (**str != '_' && !ft_isalnum(**str))
		value = ft_strdup("'$'");
	else if (**str == '0' && ++*str)
		value = ft_strdup(mini->argv[0]);
	else if (**str > '0' && **str <= '9' && ++*str)
		value = ft_strdup("");
	else
	{
		j = 0;
		identifier = ft_calloc(sizeof(char), ft_strlen(*str));
		while (**str == '_' || ft_isalnum(**str))
			identifier[j++] = *(*str)++;
		value = get_env_item(mini->envp, identifier);
		free(identifier);
	}
	*str = join_before_line(value, *str);
}
/* 
void	insert_env_item(t_mini *mini, char **str)
{
	int		j;
	char	*word;
	char	*item;

	if ((!**str || (**str > '0' && **str < '9')) && ++*str)
		return ;
	while (**str && (str[*i] < '0' || str[*i] > '9')
		&& (ft_isalnum(str[j]) || str[j] == '_'))
		j++;
	aux = ft_substr(str, *i, j - *i);
	item = get_env_item(mini->envp, aux);
	free(aux);
	str[*i -1] = '\0';
	if (item)
		aux = triple_strjoin(str, item, &str[j]);
	else
		aux = triple_strjoin(str, "", &str[j]);
	free(str);
	str = aux;
}
 */