/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 07:26:48 by math              #+#    #+#             */
/*   Updated: 2023/06/26 15:41:18 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_env_node(t_process *data, char *variable, char *value)
{
	t_env_cpy	*current;

	current = data->env_cpy;
	while (current->next != NULL)
		current = current->next;
	current->next = new_env(variable, value);
}

void	swap_node_value(t_env_cpy *a, t_env_cpy *b)
{
	char	*tmp;

	tmp = a->value;
	a->value = b->value;
	b->value = tmp;
	tmp = a->variable;
	a->variable = b->variable;
	b->variable = tmp;
}
