/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labderra <labderra@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 00:07:35 by labderra          #+#    #+#             */
/*   Updated: 2024/10/24 00:19:58 by labderra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_exit_argument(const char *str)
{
	int			sign;
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
