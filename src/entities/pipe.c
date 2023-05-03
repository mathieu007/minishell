/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 14:39:24 by math              #+#    #+#             */
/*   Updated: 2023/05/02 09:07:02 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_pipe	*new_pipe(t_cmd *cmd)
{
	t_pipe	*p;
	int32_t	fds[2];

	p = malloc(sizeof(t_pipe));
	if (p == NULL)
		return (NULL);
	if (pipe(fds) == -1)
	{
		perror("An error occur while pipping");
		free_all_and_exit();
	}
	p->fd_in = fds[0];
	p->fd_out = fds[1];
	return (p);
}

void	*free_pipe(t_cmd *cmd)
{
	if (cmd->pipe != NULL)
		free(cmd->pipe);
	cmd->pipe = NULL;
	cmd = cmd->next;
	return (NULL);
}
