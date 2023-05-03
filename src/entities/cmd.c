/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/05/03 08:17:01 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*new_cmd_after(t_cmd *curr)
{
	t_cmd	*new;

	new = malloc(sizeof(t_cmd));
	if (new == NULL)
		return (NULL);
	new->next = NULL;
	if (curr)
	{
		if (curr->next)
			new->next = curr->next;
		curr->next = new;
		new->prev = curr;
	}
	return (new);
}

inline t_cmd	*get_cmds(void)
{
	static t_cmd	*cmd;

	if (cmd == NULL)
	{
		cmd = new_cmd_after(NULL);
		return (cmd);
	}		
	return (&cmd[0]);
}

void	*free_cmd(t_cmd *cmd)
{
	free_pipe(cmd);
	free_redirect(cmd);
}
