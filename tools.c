/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labderra <labderra@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 12:21:28 by labderra          #+#    #+#             */
/*   Updated: 2024/09/16 00:54:45 by labderra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_sp(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

int	is_ctrl(char c)
{
	return (c == '<' || c == '>' || c == '|');
}

char	*join_before_line(char *prefix, char *line)
{
	char	*temp;
	int		i;
	int		j;

	temp = malloc(sizeof(char) * (ft_strlen(prefix) + ft_strlen(line) + 1));
	if (!temp)
		return (NULL);
	i = 0;
	while (prefix[i])
		temp[i++] = prefix[i++];
	j = 0;
	while (line[j])
		temp[i++] = line[j++];
	temp[i] = '\0';
	free(line);
	free(prefix);
	return (temp);
}

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

void	free_list(t_tkn **tkn_list)
{
	t_tkn	*tmp;

	while (tkn_list && *tkn_list)
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
