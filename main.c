/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labderra <labderra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 12:45:50 by labderra          #+#    #+#             */
/*   Updated: 2024/09/13 15:00:05 by labderra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_mini	*init_shell(char **envp)
{
	t_mini	*mini;

	mini = malloc(sizeof(t_mini));
	if (!mini)
		return (NULL);
	mini->envp = envp;
	mini->path = get_full_path(envp);
	if (!mini->path)
		return (NULL);
	mini->status = 0;
	mini->tkn_list = NULL;
	return (mini);
}

void	printea_la_lista_de_tokens(t_mini *mini)
{
	t_tkn	*t;
	
	t = mini->tkn_list;
	if (t == NULL)
		printf("Aqui no hay nada\n");
	while (t)
	{
		printf("tkn_type : %d\ttkn : %s\n", t->tkn_type, t->tkn);
		t = t->next;
	}
}


int	main(int argc, char **argv, char **envp)
{
	char		*str;
	t_mini		*mini;

	(void)argv;
	mini = init_shell(envp);
	if (argc != 1 || !mini)
		return (1);
	while (1)
	{
		str = readline("MeJorShell :\\>");
		add_history(str);
	//	insert_token(mini, "hola", 2);
		parse_line(mini, str);
		exec_line(mini);
		printea_la_lista_de_tokens(mini);
		free_list(&mini->tkn_list);
		free(str);
	}
	free_split(mini->envp);
	free_split(mini->path);
	return (0);
}
