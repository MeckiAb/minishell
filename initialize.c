/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labderra <labderra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 13:42:09 by labderra          #+#    #+#             */
/*   Updated: 2024/10/18 13:54:13 by labderra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	init_environment(t_mini *mini, char **envp)
{
	t_command	*cmd;
	char		*variable;
	char		*aux;

	cmd = ft_calloc(sizeof(t_command), 1);
	variable = get_env_item(envp, "SHLVL");
	aux = ft_itoa(ft_atoi(variable) + 1);
	free(variable);
	variable = ft_strjoin("export SHLVL=", aux);
	cmd->arg_array = ft_split(variable, ' ');
	run_export(mini, cmd);
	free(aux);
	free(variable);
	free_split(cmd->arg_array);
	variable = getcwd(NULL, 0);
	aux = ft_strjoin("export PWD=", variable);
	cmd->arg_array = ft_split(aux, ' ');
	run_export(mini, cmd);
	free_split(cmd->arg_array);
	free(aux);
	free(variable);
	free(cmd);
}

t_mini	*init_shell(char **argv, char **envp)
{
	t_mini	*mini;

	mini = malloc(sizeof(t_mini));
	if (!mini)
		return (NULL);
	mini->argv = argv;
	mini->envp_dictionary = copy_split_environment(envp);
	mini->envp = NULL;
	dict_to_envp(mini);
	mini->status = 0;
	mini->mini_in = dup(STDIN_FILENO);
	mini->mini_out = dup(STDOUT_FILENO);
	mini->tkn_list = NULL;
	mini->cmd_list = NULL;
	init_environment(mini, mini->envp);
	return (mini);
}
