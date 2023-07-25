/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_file_redirection2.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/07/25 10:21:59 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_out_fds(t_cmd *redir)
{
	while (redir)
	{
		if (redir->out_redir && redir->out_redir->fd > 0)
		{
			close(redir->out_redir->fd);
			redir->out_redir->fd = -1;
		}
		redir = redir->prev;
	}
}

void	close_in_fds(t_cmd *redir)
{
	while (redir)
	{
		if (redir->in_redir && redir->in_redir->fd > 0
			&& !redir->in_redir->is_here_doc)
		{
			close(redir->in_redir->fd);
			redir->in_redir->fd = -1;
		}
		redir = redir->prev;
	}
}

void	build_redir_environement(t_cmd *main, t_cmd *redir)
{
	t_process	*proc;

	proc = get_process();
	build_token_environement(redir->token);
	if (proc->errnum > 0)
		return ;
	if (redir->type == CMD_FILEOUT || redir->type == CMD_FILEOUT_APPPEND)
		redir = parse_redirect_out(main, redir);
	else if (redir->type == CMD_HEREDOC || redir->type == CMD_FILEIN)
		redir = parse_redirect_in(main, redir);
	main->redir_processed = true;
	if (has_error())
		close_files_redirections(main);
}

t_cmd	*create_fd_redir(t_cmd *main, t_cmd *redir)
{
	t_process	*proc;

	proc = get_process();
	if (!redir || proc->errnum > 0)
		return (NULL);
	build_redir_environement(main, redir);
	if (redir && redir->type == CMD_FILEOUT)
		create_redir_out(main, redir);
	else if (redir && redir->type == CMD_FILEOUT_APPPEND)
		create_redir_append_out(main, redir);
	else if (redir && redir->type == CMD_FILEIN)
		create_redir_in(main, redir);
	else if (redir && redir->type == CMD_HEREDOC)
		proc->errnum = write_here_document(redir->name, main, redir);
	if (redir->out_redir && redir->out_redir->fd > 0)
		close(redir->out_redir->fd);
	if (redir->in_redir && redir->in_redir->fd > 0)
		close(redir->in_redir->fd);
	redir = create_fd_redir(main, redir->next);
	return (redir);
}
