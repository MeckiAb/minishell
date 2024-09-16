/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labderra <labderra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 11:07:06 by labderra          #+#    #+#             */
/*   Updated: 2024/09/16 12:45:46 by labderra         ###   ########.fr       */
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


void	exec_line(t_mini *mini)
{
	printea_la_lista_de_tokens(mini);
	return ;
}
