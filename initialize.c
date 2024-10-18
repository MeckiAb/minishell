/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labderra <labderra@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 23:14:58 by labderra          #+#    #+#             */
/*   Updated: 2024/10/17 23:42:27 by labderra         ###   ########.fr       */
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
/* 	mini->path = get_full_path(mini->envp);
	if (!mini->path)
		return (NULL); */
	mini->status = 0;
	mini->mini_in = dup(STDIN_FILENO);
	mini->mini_out = dup(STDOUT_FILENO);
	mini->tkn_list = NULL;
	mini->cmd_list = NULL;
	init_environment(mini, envp);
	return (mini);
}

void	free_shell(t_mini *mini)
{
	free_commands_and_tokens(mini);
	rl_clear_history();
//	free_split(mini->path);
	free_split(mini->envp);
	free_dictionary(mini->envp_dictionary);
	close(mini->mini_in);
	close(mini->mini_out);
	free(mini);
}
