/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labderra <labderra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 12:42:56 by labderra          #+#    #+#             */
/*   Updated: 2024/10/17 18:33:35 by labderra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	sort_export(char ***env)
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

static int	print_export(t_mini *mini, t_command *cmd)
{
	int		i;
	char	***env;

	i = 0;
	apply_redir(cmd);
	env = mini->envp_dictionary;
	sort_export(env);
	while (env[i])
	{
		if (!env[i][1] || !*env[i][1])
			printf("declare -x %s\n", env[i][0]);
		else
			printf("declare -x %s=\"%s\"\n", env[i][0], env[i][1]);
		i++;
	}
	revert_redir (mini, cmd);
	return (0);
}

static int	check_export(char *str)
{
	int	i;

	i = 0;
	if (ft_isalpha(str[i]) || str[i] == '_')
		i++;
	else
		return (1);
	while (str[i] && str[i] != '=')
	{
		if (str[i] != '_' && !ft_isalnum(str[i]))
			return (1);
		i++;
	}
	return (0);
}

int	run_export(t_mini *mini, t_command *cmd)
{
	int		i;
	int		j;
	int		flag;
	char	***aux;
	char	**new;

	i = 1;
	if (!cmd->arg_array[1])
	{
		cmd->exit_status = print_export(mini, cmd);
		return (0);
	}
	while (cmd->arg_array[i])
	{
		flag = ft_strchr(cmd->arg_array[i], '=') != NULL;
		if (check_export(cmd->arg_array[i]))
		{
			printf("minishell: export:");
			printf(" ´%s': not a valid identifier\n", cmd->arg_array[i]);
			return (1);
		}
		j = 0;
		while (mini->envp_dictionary && mini->envp_dictionary[j])
			j++;
		mini->envp_dictionary = ft_realloc(mini->envp_dictionary, sizeof(char ***) * (j + 1) , sizeof(char ***) * (j + 2));
		aux = mini->envp_dictionary;
		while (aux && *aux && ft_strncmp(cmd->arg_array[i], (*aux)[0], ft_strlen((*aux)[0])))
			aux++;
		if (aux && !*aux && !flag)
		{
			new = ft_calloc(sizeof(char *), 3);
			new[0] = ft_strdup(cmd->arg_array[i]);
			new[1] = ft_strdup("\0");
			mini->envp_dictionary = add_elem_to_dict(new, mini->envp_dictionary);
		}
		if (flag)
		{
			if (aux && *aux)
				free_split(*aux);
			*aux = ft_calloc(sizeof(char *), 3);
			(*aux)[0] = ft_substr(cmd->arg_array[i], 0, len_before_equal(cmd->arg_array[i]));
			(*aux)[1] = ft_substr(cmd->arg_array[i], len_before_equal(cmd->arg_array[i]) + 1, ft_strlen(cmd->arg_array[i]));
		}
		dict_to_envp(mini);
		i++;
	}
	return (0);
}
