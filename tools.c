/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labderra <labderra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 12:21:28 by labderra          #+#    #+#             */
/*   Updated: 2024/09/10 13:47:24 by labderra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*triple_strjoin(char const *s1, char const *s2, char const *s3)
{
	size_t	i;
	char	*p;

	i = 0;
	p = (char *)malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2)
				+ ft_strlen(s3) + 1));
	if (!p)
		return (NULL);
	while (*s1)
		p[i++] = *s1++;
	while (*s2)
		p[i++] = *s2++;
	while (*s3)
		p[i++] = *s3++;
	p[i] = '\0';
	return (p);
}

static char	**get_full_path(char **envp)
{
	while (envp && *envp && ft_strncmp(*envp, "PATH=", 5))
		envp++;
	if (!envp || !*envp)
		return (NULL);
	return (ft_split(*envp + 5, ':'));
}

void	free_list(t_tkn **tkn_list)
{
	t_tkn	*tmp;

	while (*tkn_list)
	{
		tmp = *tkn_list;
		*tkn_list = (*tkn_list)->next;
		free(tmp);
	}
}

void	free_split(char **str)
{
	int	i;

	i = 0;
	while (str && str[i])
		free(str[i++]);
	free(str);
}

char	*get_env_item(char **envp, char *item)
{
	while (envp && *envp && ft_strncmp(*envp, item, ft_strlen(item)))
		envp++;
	if (!envp || !*envp)
		return (NULL);
	return (ft_strdup(*envp + ft_strlen(item)));
}

t_mini	*init_shell(char **envp)
{
	t_mini	*mini;

	mini = malloc(sizeof(t_mini));
	if (!mini)
		return (NULL);
	mini->envp = envp;
	mini->path = get_full_path(envp);
	if (!mini->path)
		return (NULL);
	mini->status = 0;
	return (mini);
}