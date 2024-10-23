/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labderra <labderra@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 12:04:43 by labderra          #+#    #+#             */
/*   Updated: 2024/10/24 00:36:07 by labderra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include <stdio.h>
# include <errno.h>
# include <unistd.h>
# include <fcntl.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <signal.h>
# include <term.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "libft/libft.h"

extern int	g_signal;

typedef struct s_tkn
{
	int				tkn_type;
	char			*tkn;
	struct s_tkn	*next;
}				t_tkn;

typedef struct s_command
{
	char				**arg_array;
	int					infile;
	int					outfile;
	int					exit_status;
	int					pid;
	struct s_command	*next;
}						t_command;

typedef struct s_mini
{
	char		**argv;
	char		**envp;
	char		***envp_dictionary;
	char		**path;
	int			status;
	int			mini_in;
	int			mini_out;
	t_tkn		*tkn_list;
	t_command	*cmd_list;
}				t_mini;

/* MAIN PROGRAM */
t_mini	*init_shell(char **argv, char **envp);
void	lexer(t_mini *mini, char *str);
void	insert_word(t_mini *mini, char **str);
void	insert_token(t_mini *mini, char *str, int tkn_type);
char	*check_syntax(t_mini *mini);
void	parser(t_mini *mini);
void	exec_line(t_mini *mini);

/* ENVIRONMENT MANAGEMENT */
char	*get_env_item(char **envp, char *item);
char	*insert_variable_value(t_mini *mini, char **str);
int		search_env(t_mini *mini, char *str);
void	dict_to_envp(t_mini *mini);
char	**copy_environment(char **envp);
char	***copy_split_environment(char **envp);

/* SYSTEM FUNCTIONS */
void	run_execve_command(t_mini *mini, t_command *cmd, int fd[2]);

/* BUILTIN FUNCTIONS */
int		run_echo(t_mini *mini, t_command *cmd);
int		run_pwd(t_mini *mini, t_command *cmd);
int		run_export(t_mini *mini, t_command *cmd);
int		run_env(t_mini *mini, t_command *cmd);
int		run_exit(t_mini *mini, t_command *cmd);
int		run_unset(t_mini *mini, t_command *cmd);
int		run_cd(t_mini *mini, t_command *cmd);

/* REDIRECTIONS & HERE-DOCUMENTS */
void	apply_redir(t_command *cmd);
void	revert_redir(t_mini *mini, t_command *cmd);
int		heredoc_launcher(t_mini *mini, char *lmt, int xpand);

/* SIGNAL HANDLERS */
void	handle_sigint_main(int sig);
void	handle_sigint_fork(int sig);
void	handle_sigquit_fork(int sig);

/* TOOLS */
void	*ft_realloc(void *p, int old_size, int new_size);
char	***add_elem_to_dict(char **str, char ***list);
char	**add_str_to_array(char *str, char **list);
char	*triple_strjoin(char const *s1, char const *s2, char const *s3);
void	print_errors(char *s1, char *s2, char *s3);
int		len_before_equal(char *s);

/* CLEANING */
void	free_shell(t_mini *mini);
void	free_split(char **str);
void	free_list(t_tkn **tkn_list);
void	free_commands_and_tokens(t_mini *mini);
void	free_dictionary(char ***dict);

#endif
