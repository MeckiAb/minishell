/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labderra <labderra@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 11:11:44 by labderra          #+#    #+#             */
/*   Updated: 2024/10/23 21:36:05 by labderra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_env_item(char **envp, char *item)
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

char	**copy_environment(char **envp)
{
	char		**p;
	int			i;

	i = 0;
	p = ft_calloc(sizeof(char *), 1);
	while (envp && envp[i])
	{
		if (ft_strncmp(envp[i], "_=", 2))
			p = add_str_to_array(envp[i], p);
		i++;
	}
	p = add_str_to_array("_=/usr/bin/env", p);
	return (p);
}

void	dict_to_envp(t_mini *mini)
{
	int		i;
	int		j;
	char	**new;

	i = 0;
	while (mini->envp_dictionary && mini->envp_dictionary[i])
		i++;
	new = ft_calloc(sizeof(char *), i + 2);
	i = 0;
	j = 0;
	while (mini->envp_dictionary && mini->envp_dictionary[i])
	{
		if (*mini->envp_dictionary[i][1])
		{
			new[j] = triple_strjoin(mini->envp_dictionary[i][0],
					"=", mini->envp_dictionary[i][1]);
			j++;
		}
		i++;
	}
	new[j] = ft_strdup("_=/usr/bin/env");
	free_split(mini->envp);
	mini->envp = new;
}
/* 
char	***triple_copy_add(char ***triple)
{
	char	***result;
	int		i;

	i = 0;
	while (triple[i])
		i++;
	result = ft_calloc(sizeof(char **), i + 1);
	i = 0;
	while (triple[i])
	{
		result[i] = copy_environment(triple[i]);
		i++;
	}
	free_dictionary(triple);
	return (result);
} */

char	***copy_split_environment(char **envp)
{
	char	***p;
	int		i;

	i = 0;
	while (envp && envp[i])
		i++;
	p = ft_calloc(sizeof(char **), i + 1);
	i = 0;
	while (envp && *envp)
	{
		if (ft_strncmp(*envp, "_=", 2))
		{
			p[i] = ft_calloc(sizeof(char *), 3);
			p[i][0] = ft_substr(*envp, 0, len_before_equal(*envp));
			p[i][1] = ft_substr(*envp,
					len_before_equal(*envp) + 1, ft_strlen(*envp));
			i++;
		}
		envp++;
	}
	return (p);
}

char	*insert_variable_value(t_mini *mini, char **str)
{
	char	*identifier;
	char	*value;
	int		j;

	if (**str == '?' && ++*str)
		value = ft_itoa(mini->status);
	else if (**str == '0' && ++*str)
		value = ft_strdup(mini->argv[0]);
	else if (**str > '0' && **str <= '9' && ++*str)
		value = ft_strdup("");
	else
	{
		j = 0;
		identifier = ft_calloc(sizeof(char), ft_strlen(*str) + 1);
		while (**str == '_' || ft_isalnum(**str))
			identifier[j++] = *(*str)++;
		value = get_env_item(mini->envp, identifier);
		free(identifier);
	}
	return (value);
}
