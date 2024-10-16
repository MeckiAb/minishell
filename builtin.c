/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labderra <labderra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 11:42:34 by jormoral          #+#    #+#             */
/*   Updated: 2024/10/08 14:19:58 by labderra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void	print_cmd(t_mini *mini)
{
	int	i = 0;
	t_command	*cmd;
	t_tkn	*tkn;

	tkn = mini->tkn_list;
	while (tkn)
	{
		printf("%s\n", tkn->tkn);
		tkn = tkn->next;
	}
	cmd = mini->cmd_list;
	printf("CMD : %p\n", cmd);
	while (cmd->arg_array[i])
		printf("ARG_ARRAY[i] : %s\n",cmd->arg_array[i++]);
	printf("PATH : %s\n", cmd->path);
	printf("INFILE : %d - OUTFILE : %d\n", cmd->infile, cmd->outfile);
	printf("PID : %d - EXIT STATUS : %d\n", cmd->pid, cmd->exit_status);
	printf("NEXT : %p\n", cmd->next);
}

int	run_pwd(t_mini *mini, t_command *cmd)
{
	char	*pwd;

	apply_redir(cmd);
	pwd = getcwd(NULL, 0);
	if(!pwd)
	{
		revert_redir(mini, cmd);
		return (1);
	}
	else
	{
		printf("%s\n", pwd);
		free (pwd);
		revert_redir(mini, cmd);
		return (0);
	}
}

int run_echo(t_mini *mini, t_command *cmd)
{
	int n_flag;
	int i;

	apply_redir(cmd);
	i = 0;
	n_flag = 0;
	if (cmd->arg_array[1] && cmd->arg_array[1][i++] == '-')
	{
		while (cmd->arg_array[1][i] == 'n')
			i += 1;
		n_flag = cmd->arg_array[1][i] == '\0';
	}
	i = 1 + n_flag;
	while(cmd->arg_array[i])
	{
		write(1, cmd->arg_array[i], ft_strlen(cmd->arg_array[i]));
		if(cmd->arg_array[++i])
			write(1, " ", 1);
	}
	if(n_flag == 0)
		write(1, "\n", 1);
	revert_redir(mini, cmd);
	return(0);
}

int	run_env(t_mini *mini, t_command *cmd)
{
	char	**aux;

	apply_redir(cmd);
	aux = mini->envp;
	if (!aux)
	{
		revert_redir(mini, cmd);
		return (1);
	}
	while (aux && *aux)
		printf("%s\n", *aux++);
	revert_redir(mini, cmd);
	return (0);
}

static int	check_exit_argument(const char *str)
{
	int	sign;
	long long	acc;

	sign = 1;
	acc = 0;
	while (*str == '\n' || *str == '\t' || *str == '\v' || *str == '\f'
		|| *str == '\r' || *str == ' ')
		str++;
	if (!ft_strncmp(str, "-9223372036854775808\0", 21))
		return (0);
	if (*str == '+' || *str == '-')
		sign -= 2 * *str++ == '-';
	if (!*str)
		return (-1);
	while (*str && acc >= 0)
	{
		if (*str >= '0' && *str <= '9')
			acc = acc * 10 + *str++ - 48;
		else
			return (-1);
	}
	if (acc < 0)
		return (-1);
	return ((sign * acc) % 256);
}

int	run_exit(t_mini *mini, t_command *cmd)
{
	apply_redir(cmd);
	if (!cmd->arg_array[1])
	{
		printf("exit\n");
		exit(0);
	}
	if (cmd->arg_array[1] && check_exit_argument(cmd->arg_array[1]) == -1)
	{
		print_errors("exit\nminishell: exit: ", cmd->arg_array[1],
			": numeric argument required\n");
		exit(2);
	}
	if (cmd->arg_array[1] && cmd->arg_array[2])
		return (revert_redir(mini, cmd), print_errors("exit\n", "",
			"minishell: exit: : too many arguments\n"), 1);
	printf("exit\n");
	exit(check_exit_argument(cmd->arg_array[1]));
	revert_redir(mini, cmd);
}

int	check_export(char *str, int *flag)
{
	int	i;

	i = 0;
	if (ft_isalpha(str[i]) || str[i] == '_')
		i++;
	else
		return (0);
	while (str[i] && str[i] != '=')
	{
		if (str[i] != '_' && !ft_isalnum(str[i]))
			return (0);
		i++;
	}
	if (str[i] == '=')
		*flag = 1;
	return (1);
}

int	search_env(t_mini *mini, char *str)
{
	int	i;

	i = 0;
	while (mini->envp[i])
	{
		if (!ft_strncmp(mini->envp[i], str, ft_strlen(str)))
			break ;
		i++;
	}
	return (i);
}

void ok_replace(t_mini *mini, t_command *cmd, int i, int na)
{
	int		i_env;

	i_env = 0;
	free(mini->envp_dictionary[i][1]);
	mini->envp_dictionary[i] = ft_split(cmd->arg_array[na], '=');
	i_env = search_env(mini, mini->envp_dictionary[i][0]);
	if (!mini->envp[i_env])
		mini->envp = add_str_to_array(cmd->arg_array[na], mini->envp);
	else
	{
		free(mini->envp[i_env]);
		mini->envp[i_env] = ft_strdup(cmd->arg_array[na]);
	}
}

void ko_replace(t_mini *mini, t_command *cmd, int i, int na)
{
	mini->envp_dictionary = triple_copy_add(mini->envp_dictionary);
	mini->envp_dictionary[i] = ft_split(cmd->arg_array[na], '=');
	mini->envp = add_str_to_array(cmd->arg_array[na], mini->envp);
}

void replace_export(t_mini *mini, t_command *cmd, int na, int flag)
{
	int		i;
	int		len;

	len = 0;
	i = 0;
	while (cmd->arg_array[na][len] && cmd->arg_array[na][len] != '=')
		len++;
	while (mini->envp_dictionary[i])
	{
		if (!ft_strncmp(mini->envp_dictionary[i][0], cmd->arg_array[na], len))
			break;
		i++;
	}
	if(mini->envp_dictionary[i] && flag)
		ok_replace(mini, cmd, i, na);
	else
	{
		if (flag)
			ko_replace(mini, cmd, i, na);
		else
		{
			mini->envp_dictionary = triple_copy_add(mini->envp_dictionary);
			mini->envp_dictionary[i] = ft_split(cmd->arg_array[na], '=');
		}
	}
}

int	only_export(t_mini *mini, t_command *cmd)
{
	int		i;
	char	***env;

	i = 0;
	apply_redir(cmd);
	env = mini->envp_dictionary;
	alpha_export(env);
	while (env[i])
	{
		if (!env[i][1])
			printf("declare -x %s\n", env[i][0]);
		else
			printf("declare -x %s=\"%s\"\n", env[i][0], env[i][1]);
		i++;
	}
	revert_redir (mini, cmd);
	return (0);
}

void	alpha_export(char ***env)
{
	int		c;
	int		i;
	char	**temp;

	i = 0;
	while (env[i] && env[i + 1])
	{
		c = 0;
		while (env[i][0][c] == env[i + 1][0][c])
			c++;
		if (env[i][0][c] > env[i + 1][0][c])
		{
			temp = env[i];
			env[i] = env[i + 1];
			env[i + 1] = temp;
			i = 0;
		}
		else
			i++;
	}
}

int	run_export(t_mini *mini, t_command *cmd)
{
	int		i;
	int		flag;

	flag = 0;
	i = 1;
	if (cmd->arg_array && !cmd->arg_array[1])
		cmd->exit_status = only_export(mini, cmd);
	while (cmd->arg_array[i])
	{
		if (!check_export(cmd->arg_array[i], &flag))
		{
			printf("minishell: export:");
			printf(" ´%s': not a valid identifier\n", cmd->arg_array[i]);
			cmd->exit_status = 1;
		}
		else
			replace_export(mini, cmd, i, flag);
		i++;
	}
	return (cmd->exit_status);
}
