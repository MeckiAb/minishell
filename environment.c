/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labderra <labderra@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 11:11:44 by labderra          #+#    #+#             */
/*   Updated: 2024/09/16 01:06:48 by labderra         ###   ########.fr       */
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
		return (NULL);
	return (ft_strdup(*envp + ft_strlen(item) + 1));
}
/* 
void	insert_env_item(t_mini *mini, char *str, int *i)
{
	int		j;
	char	*aux;
	char	*item;

	j = *i;
	while (str && (str[*i] < '0' || str[*i] > '9')
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
