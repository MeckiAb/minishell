/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labderra <labderra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 12:04:43 by labderra          #+#    #+#             */
/*   Updated: 2024/09/20 13:20:41 by labderra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include <stdio.h>
# include <unistd.h>
# include <fcntl.h>
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
	char		**argv;
	char		**envp;
	char		**path;
	int			status;
	t_tkn		*tkn_list;
	t_command	*cmd_list;
}				t_mini;

typedef struct s_command
{
	char				*path;     // el path correcto del comando
	char 				**arg_array;   // la segunda y siguiente palabras
	int					infile;    // 0 o el fd de la ultima redirección de entrada
	int					outfile;    // 1 o el fd de última redirección de salida
	int					exit_status;
	struct s_command	*next;
}						t_command;



char	**get_full_path(char **envp);

void	free_split(char **str);
void	free_list(t_tkn **tkn_list);
char	*join_before_line(char *prefix, char *line);

void	lexer(t_mini *mini, char *str);
void	parser(t_mini *mini);
void	exec_line(t_mini *mini);

void	insert_variable_value(t_mini *mini, char **str);
char	*check_syntax(t_mini *mini);
char	**add_str_to_array(char *str, char **list);
char	**copy_environmet(char **envp);


#endif
