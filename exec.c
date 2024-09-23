/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labderra <labderra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 11:07:06 by labderra          #+#    #+#             */
/*   Updated: 2024/09/23 12:37:46 by labderra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	printea_la_lista_de_tokens(t_mini *mini)
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

static void	printea_la_lista_de_commands(t_mini *mini)
{
	t_command	*t;
	int			i;
	
	t = mini->cmd_list;
	if (t == NULL)
		printf("\nFin de la Lista\n\n");
	while (t != NULL)
	{
		i = 0;
		printf("\n");
		while (t->arg_array[i])
			printf("%s ", t->arg_array[i++]);
		printf("\nInfile : %d\nOutfile : %d\n", t->infile, t->outfile);
		printf("next : %p\n", t->next);
		t = t->next;
	}
}


void	exec_line(t_mini *mini)
{
	printea_la_lista_de_tokens(mini);
	printea_la_lista_de_commands(mini);
	return ;
}
