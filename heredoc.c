/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labderra <labderra@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 23:49:23 by labderra          #+#    #+#             */
/*   Updated: 2024/10/18 00:29:46 by labderra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
			insert_variable_value(mini, str);
		}
		else
			tmp[i++] = *(*str)++;
	}
	free(backup);
	return (tmp);
}

static int	heredoc(t_mini *mini, char *lmt, int xpand)
{
	char	*aux_str;
	int		fd[2];
	int		size;

	size = ft_strlen(lmt);
	if (pipe(fd) == -1)
		return (-1);
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
	return (fd[0]);
}

int	heredoc_launcher(t_mini *mini, char *lmt, int xpand)
{
	int	pid;
	int	status;
	int	*result;

	status = 0;
	result = malloc(sizeof(int));
	*result = -1;
	pid = fork();
	if (!pid)
	{
		signal(SIGINT, SIG_DFL);
		*result = heredoc(mini, lmt, xpand);
		exit(0);
	}
	else
	{
		signal(SIGINT, handle_sigint_fork);
		waitpid(pid, &status, 0);
	}
	return (*result);
}
