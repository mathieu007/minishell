/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/05/09 14:06:44 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int32_t	pipe_first_child(t_cmd *cmd)
{
	dup2(cmd->redirect->fd_out, STDOUT_FILENO);
	close_pipe_fds(cmd);
	// execute(cmd);
	return (1);
}

int32_t	pipe_last_child(t_cmd *cmd)
{
	dup2(cmd->redirect->fd_out, STDOUT_FILENO);
	close_pipe_fds(cmd);
	// execute(cmd);
	return (1);
}
