/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labderra <labderra@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 12:21:28 by labderra          #+#    #+#             */
/*   Updated: 2024/10/24 00:31:12 by labderra         ###   ########.fr       */
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

void	*ft_realloc(void *p, int old_size, int new_size)
{
	void	*r;

	if (!new_size)
		return (free(p), NULL);
	r = ft_calloc(1, new_size);
	if (p && old_size <= new_size)
		ft_memmove(r, p, old_size);
	else if (p && old_size > new_size)
		ft_memmove(r, p, new_size);
	return (free(p), r);
}

char	**add_str_to_array(char *str, char **list)
{
	int		size;
	char	**r;

	size = 0;
	while (list && list[size])
		size++;
	size += 1;
	r = ft_realloc(list, sizeof(char *) * size, sizeof(char *) * (size + 1));
	r[size - 1] = ft_strdup(str);
	return (r);
}

char	***add_elem_to_dict(char **str, char ***list)
{
	int		size;
	char	***r;

	size = 0;
	while (list && list[size])
		size++;
	size += 1;
	r = ft_realloc(list, sizeof(char **) * size, sizeof(char **) * (size + 1));
	r[size - 1] = str;
	return (r);
}
