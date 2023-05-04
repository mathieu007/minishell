/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/05/02 10:32:20 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int32_t	pipe_first_child(t_cmd *cmd)
{
	dup2(cmd->redirect->file_out, STDOUT_FILENO);
	close_pipe_fds(cmd);
	execute(cmd);
}

int32_t	pipe_first_child(t_cmd *cmd)
{
	dup2(cmd->redirect->file_out, STDOUT_FILENO);
	close_pipe_fds(cmd);
	execute(cmd);
}
