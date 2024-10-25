/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labderra <labderra@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 23:49:23 by labderra          #+#    #+#             */
/*   Updated: 2024/10/25 07:33:14 by labderra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	expand_word(t_mini *mini, char **str, char **tmp)
{
	int		j;
	char	*value;
	char	*aux;

	value = insert_variable_value(mini, str);
	j = ft_strlen(value);
	aux = *tmp;
	*tmp = ft_strjoin(aux, value);
	free(aux);
	free(value);
	return (j);
}

static char	*expand_heredoc_dollar(t_mini *mini, char **str)
{
	int		i;
	char	*tmp;
	char	*backup;

	i = 0;
	tmp = ft_calloc(sizeof(char), 4096);
	if (!tmp)
		return (NULL);
	backup = *str;
	while (*str && **str)
	{
		if (**str == '$' && (ft_isalnum(*(*str + 1)) || *(*str + 1) == '_'))
		{
			++*str;
			i += expand_word(mini, str, &tmp);
		}
		else
			tmp[i++] = *(*str)++;
	}
	free(backup);
	return (tmp);
}

static void	heredoc(t_mini *mini, char *lmt, int xpand, int fd[2])
{
	char	*aux_str;
	int		size;

	size = ft_strlen(lmt);
	close(fd[0]);
	while (1)
	{
		aux_str = readline("heredoc>");
		if (!aux_str)
			break ;
		if (!ft_strncmp(aux_str, lmt, size + 1))
			break ;
		if (xpand)
			aux_str = expand_heredoc_dollar(mini, &aux_str);
		write(fd[1], aux_str, ft_strlen(aux_str));
		write(fd[1], "\n", 1);
		free(aux_str);
	}
	free(aux_str);
	close(fd[1]);
}

int	heredoc_launcher(t_mini *mini, char *lmt, int xpand)
{
	int	pid;
	int	status;
	int	fd[2];

	status = 0;
	if (pipe(fd) == -1)
		return (-1);
	pid = fork();
	if (!pid)
	{
		signal(SIGINT, SIG_DFL);
		heredoc(mini, lmt, xpand, fd);
		exit(0);
	}
	else
	{
		signal(SIGINT, handle_sigint_fork);
		waitpid(pid, &status, 0);
	}
	close(fd[1]);
	return (fd[0]);
}
