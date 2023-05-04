/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   close.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/05/02 10:13:32 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief close pipes before execve
/// @param cmd 
void	close_pipe_fds(t_cmd *cmd)
{
	if (cmd->redirect)
	{
		while (cmd && cmd->redirect)
		{
			if (cmd->redirect->fd_in != -1)
				close(cmd->redirect->fd_out);
			if (cmd->redirect->fd_out != -1)
				close(cmd->redirect->fd_out);
			cmd = cmd->next;
		}
	}
	if (cmd->pipe->fd_in != -1)
		close(cmd->pipe->fd_in);
	if (cmd->pipe->fd_in != -1)
		close(cmd->pipe->fd_in);
}
