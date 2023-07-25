/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/07/25 17:13:30 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_prev_pipes(t_cmd *cmd)
{
	if (!cmd)
		return ;
	cmd = cmd->prev;
	if (cmd && cmd->pipe)
	{
		if (cmd->pipe->fd_in > 2)
		{
			close(cmd->pipe->fd_in);
			cmd->pipe->fd_in = -1;
		}
		if (cmd->pipe->fd_out > 2)
		{
			close(cmd->pipe->fd_out);
			cmd->pipe->fd_out = -1;
		}
	}
}

t_pipe	*prev_pipe(t_cmd *cmd)
{
	if (!cmd)
		return (NULL);
	cmd = cmd->prev;
	if (cmd)
		return (cmd->pipe);
	return (NULL);
}

void	close_pipes(t_pipe *pipe)
{
	if (pipe->fd_in > 2)
	{
		close(pipe->fd_in);
		pipe->fd_in = -1;
	}
	if (pipe->fd_out > 2)
	{
		close(pipe->fd_out);
		pipe->fd_out = -1;
	}
}

t_pipe	*init_pipes(int32_t *fds, t_cmd *cmd)
{
	cmd->pipe = malloc(sizeof(t_cmd));
	if (!cmd->pipe)
		return (NULL);
	cmd->pipe->fd_in = fds[0];
	cmd->pipe->fd_out = fds[1];
	return (cmd->pipe);
}

void	pipe_cmd(t_cmd *cmd)
{
	int32_t	fds[2];

	if (pipe(fds) == -1)
		free_all_and_exit2(errno, "Pipe error");
	init_pipes(fds, cmd);
}
