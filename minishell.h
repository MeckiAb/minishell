/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labderra <labderra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 12:04:43 by labderra          #+#    #+#             */
/*   Updated: 2024/09/18 11:48:37 by labderra         ###   ########.fr       */
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
	int		status;
	t_tkn	*tkn_list;
}				t_mini;

typedef struct s_command_list
{
	char					*command;     // la primera palabra que me encuentre
	char 					**arguments;   // la segunda y siguiente palabras
	char					*environment;
	int						command_infile;    // 0 o el fd de la ultima redirección de entrada
	int						command_outfile;    // 1 o el fd de última redirección de salida
	int						exit_status;
	struct s_command_list	*next;
}							t_command_list;



char	**get_full_path(char **envp);

void	free_split(char **str);
void	free_list(t_tkn **tkn_list);
char	*join_before_line(char *prefix, char *line);

void	parse_line(t_mini *mini, char *str);
void	exec_line(t_mini *mini);

void	insert_variable_value(t_mini *mini, char **str);
char	*check_syntax(t_mini *mini);

#endif
