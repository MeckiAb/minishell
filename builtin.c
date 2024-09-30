/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labderra <labderra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 11:42:34 by jormoral          #+#    #+#             */
/*   Updated: 2024/09/30 19:47:43 by labderra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	run_pwd(t_mini *mini, t_command *cmd)
{
	//quizas tengo que mandarle t_mini para que pueda sacar el pwd
	//de la copia del enviroment
	// y en vez de retornar un entero que se guarde mini->status
	char *pwd;
	
	if (!mini)
		return (1);
	if (!cmd)
		return (1);
	
	pwd = getcwd(NULL, 0);
	if(!pwd)
		return (1);
	else
	{
		printf("%s\n", pwd);
		free (pwd);
		return (0);
	}
}

int run_echo(t_command *cmd)
{
	int nflag;
	int i;
	int x;
	x = 1;
	i = 1;
	nflag = 0;
	while(cmd->arg_array[1][x] && cmd->arg_array[1][0] == '-')
	{
		if(cmd->arg_array[1][x] != 'n')
		{
			nflag = 0;
			break;
		}
		x++;
		nflag = 1;
	}
	if(nflag)
		i++;
	while(cmd->arg_array[i])
	{
		ft_putstr_fd(cmd->arg_array[i], cmd->outfile);
		if(cmd->arg_array[i + 1])
			ft_putstr_fd(" ", cmd->outfile);
		i++;
	}
	if(nflag == 0)
		ft_putstr_fd("\n", cmd->outfile);
	return(0);
} 
