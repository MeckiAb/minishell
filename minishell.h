/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labderra <labderra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 12:04:43 by labderra          #+#    #+#             */
/*   Updated: 2024/09/10 19:14:38 by labderra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "libft/libft.h"

typedef struct	s_tkn
{
	int				tkn_type;
	char			*tkn;
	struct s_tkn	*next;
}				t_tkn;

typedef struct	s_mini
{
	char	**envp;
	char	**path;
	t_tkn	*tkn_list;
	int		status;
}				t_mini;

static char	**get_full_path(char **envp);
void	free_split(char **str);
void	free_list(t_tkn *tkn_list);
t_mini	*init_shell(char **envp);
void	parse_line(t_mini *mini, char *str);
void	exec_line(t_mini *mini);
char	*get_env_item(char **envp, char *item);

#endif
