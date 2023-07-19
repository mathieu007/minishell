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
	proc->errnum = 0;
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

// void	*close_parent_subshell_fds(t_cmd *cmd)
// {
// 	t_process	*proc;

// 	proc = get_process();
// 	if (proc->is_subshell && cmd->has_redirection)
// 	{
// 		if (proc->parent_in_fd >= 3 && cmd->in_redir->fd >= 3)
// 			close(proc->parent_in_fd);
// 		if (proc->parent_out_fd >= 3 && cmd->out_redir->fd >= 3)
// 			close(proc->parent_out_fd);
// 	}		
// 	return (NULL);
// }

t_cmd	*create_fd_redir(t_cmd *main, t_cmd *redir)
{
	t_process	*proc;

	proc = get_process();
	if (!redir)
		return (NULL);
	build_redir_environement(main, redir);
	if (redir && redir->type == CMD_FILEOUT)
		create_redir_out(main, redir);
	else if (redir && redir->type == CMD_FILEOUT_APPPEND)
		create_redir_append_out(main, redir);
	else if (redir && redir->type == CMD_FILEIN)
		create_redir_in(main, redir);
	else if (redir && redir->type == CMD_HEREDOC)
	{
		create_redir_heredoc(main, redir);
		proc->errnum = write_here_document(redir->name, main);
	}
	if (redir->out_redir && redir->out_redir->fd > 0)
		close(redir->out_redir->fd);
	if (redir->in_redir && redir->in_redir->fd > 0)
		close(redir->in_redir->fd);
	redir = create_fd_redir(main, redir->next);
	return (redir);
}
