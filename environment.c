/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labderra <labderra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 11:11:44 by labderra          #+#    #+#             */
/*   Updated: 2024/09/16 20:01:45 by labderra         ###   ########.fr       */
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
	char	*search;
	
	search = ft_strjoin(item, "=");
	if (!search)
		return (item);
	while (envp && *envp && ft_strncmp(*envp, search, ft_strlen(search)))
		envp++;
	free(search);
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