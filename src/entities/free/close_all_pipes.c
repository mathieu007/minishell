/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   close_all_pipes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 07:26:48 by math              #+#    #+#             */
/*   Updated: 2023/07/26 15:33:18 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_child_pipes(t_cmd *cmd)
{
	while (cmd)
	{
		if (cmd->pipe && cmd->pipe->fd_in > 2)
		{
			close(cmd->pipe->fd_in);
			cmd->pipe->fd_in = -1;
		}
		if (cmd->pipe && cmd->pipe->fd_out > 2)
		{
			close(cmd->pipe->fd_out);
			cmd->pipe->fd_out = -1;
		}
		cmd = cmd->next;
	}
}

void	close_all_pipes(void)
{
	t_cmd		*cmd;
	t_process	*proc;

	proc = get_process();
	cmd = proc->cmds;
	while (cmd)
	{
		if (cmd->pipe && cmd->pipe->fd_in > 2)
		{
			close(cmd->pipe->fd_in);
			cmd->pipe->fd_in = -1;
		}
		if (cmd->pipe && cmd->pipe->fd_out > 2)
		{
			close(cmd->pipe->fd_out);
			cmd->pipe->fd_out = -1;
		}
		close_child_pipes(cmd->child);
		cmd = cmd->next;
	}
}
