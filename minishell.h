/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labderra <labderra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 12:04:43 by labderra          #+#    #+#             */
/*   Updated: 2024/09/17 13:31:06 by labderra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "libft/libft.h"

typedef struct s_tkn
{
	int				tkn_type;
	char			*tkn;
	struct s_tkn	*next;
}				t_tkn;

typedef struct s_mini
{
	char	**argv;
	char	**envp;
	char	**path;
	t_tkn	*tkn_list;
	int		status;
}				t_mini;

char	**get_full_path(char **envp);
/* int		is_sp(char c);
int		is_ctrl(char c);
 */
void	free_split(char **str);
void	free_list(t_tkn **tkn_list);
char	*join_before_line(char *prefix, char *line);


void	parse_line(t_mini *mini, char *str);
void	exec_line(t_mini *mini);

//char	*triple_strjoin(char const *s1, char const *s2, char const *s3);
//void	insert_env_item(t_mini *mini, char *str, int *i);
void	insert_token(t_mini *mini, char *str, int tkn_type);
void	insert_variable_value(t_mini *mini, char **str);

void	del_empty_tokens(t_tkn **tkn_list);


#endif
