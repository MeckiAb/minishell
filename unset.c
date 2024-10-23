/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labderra <labderra@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 15:36:35 by jormoral          #+#    #+#             */
/*   Updated: 2024/10/23 23:30:10 by labderra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**unset_envp(t_mini *mini, t_command *cmd, char **temp)
{
	int	i;
	int	j;
	int	t;

	t = 0;
	i = 0;
	while (mini->envp[i])
	{
		j = 1;
		while (cmd->arg_array[j])
		{
			if (!ft_strncmp(mini->envp[i], cmd->arg_array[j],
					len_before_equal(mini->envp[i])))
			{
				free(mini->envp[i]);
				mini->envp[i] = NULL;
				break ;
			}
			j++;
		}
		if (!cmd->arg_array[j])
			temp[t++] = mini->envp[i];
		i++;
	}
	return (temp);
}

static char	***unset_export(t_mini *mini, t_command *cmd, char ***temp)
{
	int	i;
	int	j;
	int	t;

	t = 0;
	i = 0;
	while (mini->envp_dictionary[i] && mini->envp_dictionary[i][0])
	{
		j = 1;
		while (cmd->arg_array[j] && !ft_strchr(cmd->arg_array[j], '='))
		{
			if (!ft_strncmp(mini->envp_dictionary[i][0], cmd->arg_array[j],
				ft_strlen(mini->envp_dictionary[i][0])))
			{
				free_split(mini->envp_dictionary[i]);
				mini->envp_dictionary[i] = NULL;
				break ;
			}
			j++;
		}
		if (!cmd->arg_array[j] || ft_strchr(cmd->arg_array[j], '='))
			temp[t++] = mini->envp_dictionary[i];
		i++;
	}
	return (temp);
}

int	run_unset(t_mini *mini, t_command *cmd)
{
	int		len;
	char	**tmp_env;
	char	***tmp_exp;

	len = 0;
	while (mini->envp[len])
		len++;
	tmp_env = ft_calloc(len + 1, (sizeof(char *)));
	len = 0;
	while (mini->envp_dictionary[len])
		len++;
	tmp_exp = ft_calloc(len + 1, (sizeof(char **)));
	tmp_exp = unset_export(mini, cmd, tmp_exp);
	free(mini->envp_dictionary);
	mini->envp_dictionary = tmp_exp;
	tmp_env = unset_envp(mini, cmd, tmp_env);
	free(mini->envp);
	mini->envp = tmp_env;
	return (0);
}
