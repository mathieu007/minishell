/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_redirections.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/07/26 15:43:23 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	file_redirection(t_cmd *cmd, bool is_in_child_process)
{
	if (!cmd)
		return ;
	if (cmd->has_redirection && cmd->in_redir)
		redirect_input(cmd, is_in_child_process);
	if (cmd->has_redirection && cmd->out_redir)
		redirect_output(cmd, is_in_child_process);
}

void	unlink_files_redirections(t_redirect *redir)
{
	if (redir && (redir->fd_is_temp || redir->is_here_doc))
		unlink(redir->file);
}

void	close_files_redirections(t_cmd *cmd)
{
	if (!cmd)
		return ;
	if (cmd->in_redir && cmd->in_redir->fd > 2)
	{
		if (cmd->in_redir->fd > 2)
			close(cmd->in_redir->fd);
		cmd->in_redir->fd = -1;
		cmd->in_redir->dup_fd = -1;
	}
	if (cmd->out_redir && cmd->out_redir->fd > 2)
	{
		if (cmd->out_redir->dup_fd >= 0)
			dup2(cmd->out_redir->dup_fd, STDOUT_FILENO);
		if (cmd->out_redir->fd > 2)	
			close(cmd->out_redir->fd);
		cmd->out_redir->fd = -1;
		cmd->out_redir->dup_fd = -1;
	}
}

void	redirect_input(t_cmd *cmd, bool is_in_child_process)
{
	if (!cmd || !cmd->in_redir)
		return ;
	cmd->in_redir->fd = open(cmd->in_redir->file, O_RDONLY, 0777);
	if (dup2(cmd->in_redir->fd, STDIN_FILENO) == -1)
		free_all_and_exit2(errno, "Could not redirect input");
	if (cmd->type == CMD && is_in_child_process)
	{
		if (cmd->in_redir->fd > 2 && close(cmd->in_redir->fd) == -1)
			free_all_and_exit2(errno, "Could not close the fd");
		cmd->in_redir->fd = -1;
	}
}

void	redirect_output(t_cmd *cmd, bool is_in_child_process)
{
	if (!cmd || !cmd->out_redir)
		return ;
	cmd->out_redir->fd = open(cmd->out_redir->file, cmd->out_redir->flags,
			0777);
	if (!is_in_child_process)
		cmd->out_redir->dup_fd = dup(STDOUT_FILENO);
	if (dup2(cmd->out_redir->fd, STDOUT_FILENO) == -1)
		free_all_and_exit2(errno, "Could not redirect output");
	if ((cmd->type == CMD || cmd->type == CMD_PARENTHESES)
		&& is_in_child_process)
	{
		if (cmd->out_redir->fd > 2 && close(cmd->out_redir->fd) == -1)
			free_all_and_exit2(errno, "Could not close the fd");
		cmd->out_redir->fd = -1;
	}
}
