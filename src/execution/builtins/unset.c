/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: math <math@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 07:26:48 by math              #+#    #+#             */
/*   Updated: 2023/07/21 20:37:49 by math             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	remove_node_from_list(t_process *data, t_env_cpy *node, t_env_cpy *prev)
{
	if (prev)
		prev->next = node->next;
	else
		data->env_cpy = node->next;
	if (node->next)
		node->next->prev = prev;
	free(node);
}

void	remove_env_var(t_process *data, char *variable)
{
	t_env_cpy	*current;
	t_env_cpy	*prev;
	size_t		len;

	current = data->env_cpy;
	prev = NULL;
	while (current)
	{
		len = ft_strlen(variable);
		if (ft_strncmp(variable, current->variable, len) == 0)
		{
			remove_node_from_list(data, current, prev);
			break ;
		}
		prev = current;
		current = current->next;
	}
}

int	unset_cmd(t_cmd *cmd)
{
	t_process	*data;
	int			i;

	data = get_process();
	if (cmd->options != NULL)
	{
		ft_printf("Error: Unset option not handled.\n");
		return (1);
	}
	i = 1;
	while (cmd->args[i])
	{
		if (is_valid_identifier(cmd->args[i]) == 0)
		{
			print_not_valid_identifier(1, cmd->args[i]);
			i++;
			continue ;
		}
		remove_env_var(data, cmd->args[i]);
		i++;
	}
	return (0);
}
