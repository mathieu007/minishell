/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: math <math@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/06/06 07:10:42 by math             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*new_cmd()
{
	t_cmd	*new;

	new = ft_calloc(1, sizeof(t_cmd));
	new->pipe = NULL;
	if (new == NULL)
		return (NULL);
	return (new);
}

inline t_cmd	*add_cmd(void)
{
	t_cmd		*last;
	t_cmd		*new;
	t_process	*data;

	data = get_process();
	last = data->last_cmd;
	new = new_cmd();
	if (new == NULL)
		return (NULL);
	if (!last)
		data->cmds = new;
	else
	{	
		last->next = new;
		new->prev = last;
	}
	// data->tokens_count++;
	data->last_cmd = new;
	return (new);
}

void	*free_cmd(t_cmd *cmd)
{
	free_pipe(cmd);
	free_redirect(cmd);
	return (NULL);
}
