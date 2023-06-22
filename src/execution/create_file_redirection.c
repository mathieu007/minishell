#include "minishell.h"

bool	is_redirection(t_cmd_seq seq)
{
	return (seq && (seq == CMD_FILEIN
			|| seq == CMD_FILEOUT
			|| seq == CMD_FILEOUT_APPPEND
			|| seq == CMD_HEREDOC));
}

bool	is_token_redir(t_token_type type)
{
	return (type && (type == TK_GREAT
			|| type == TK_GREATGREAT
			|| type == TK_LESS
			|| type == TK_LESSLESS));
}

// int32_t count_args2(char **args)
// {
// 	int32_t count;

// 	count = 0;
// 	if (args)
// 	{
// 		while (args[count])
// 			count++;
// 	}
// 	return (count);
// }

void	copy_redirection(t_redirect *dest, t_redirect *src)
{
	dest->fd = src->fd;
	dest->fd_is_temp = src->fd_is_temp;
	if (dest->file)
		free(dest->file);
	if (dest->input_file)
		free(dest->input_file);
	dest->file = ft_strdup(src->file);
	dest->input_file = ft_strdup(src->input_file);
}

t_cmd	*create_redir_out(t_cmd *main, t_cmd *redir)
{
	t_process	*proc;

	proc = get_process();
	proc->errnum = 0;
	build_redir_token_environement(redir->token, redir->type);
	if (proc->errnum > 0)
		return (redir->next);
	redir = parse_redirect_out(main, redir);
	open_out_redir_fd(redir);
	main->out_redir = new_redirect();
	copy_redirection(main->out_redir, redir->out_redir);
	return (redir);
}

t_cmd	*create_redir_append_out(t_cmd *main, t_cmd *redir)
{
	t_process	*proc;

	proc = get_process();
	proc->errnum = 0;
	build_redir_token_environement(redir->token, redir->type);
	if (proc->errnum > 0)
		return (redir->next);
	redir = parse_redirect_out(main, redir);
	open_out_append_redir_fd(redir);
	copy_redirection(main->out_redir, redir->out_redir);
	return (redir);
}

t_cmd	*create_redir_in(t_cmd *main, t_cmd *redir)
{
	t_process	*proc;

	proc = get_process();
	proc->errnum = 0;
	build_redir_token_environement(redir->token, redir->type);
	if (proc->errnum > 0)
		return (redir->next);
	redir = parse_redirect_in(main, redir);
	open_in_redir_fd(redir);
	main->in_redir = new_redirect();
	copy_redirection(main->in_redir, redir->in_redir);
	return (redir);
}

t_cmd	*create_redir_heredoc(t_cmd *main, t_cmd *redir)
{
	t_process	*proc;

	proc = get_process();
	proc->errnum = 0;
	build_redir_token_environement(redir->token, redir->type);
	if (proc->errnum > 0)
		return (redir->next);
	redir = parse_redirect_in(main, redir);
	open_in_redir_fd(redir);
	copy_redirection(main->in_redir, redir->in_redir);
	return (redir);
}

void	close_unnecessary_out_fd(t_cmd *redir)
{
	int32_t	count;

	count = 0;
	while (redir)
	{
		if (redir->out_redir && redir->out_redir->fd > 0)
			count++;
		if (count > 1 && redir->out_redir)
		{
			close(redir->out_redir->fd);
			redir->out_redir->fd = -1;
		}
		redir = redir->prev;
	}
}

void	close_unnecessary_in_fd(t_cmd *redir)
{
	int32_t	count;

	count = 0;
	while (redir)
	{
		if (redir->in_redir && redir->in_redir->fd > 0)
			count++;
		if (count > 1 && redir->in_redir)
		{
			close(redir->in_redir->fd);
			redir->in_redir->fd = -1;
		}
		redir = redir->prev;
	}
}

/// @brief at first main == cmd
/// @param main 
/// @param cmd 
/// @return
t_cmd	*create_fd_redir(t_cmd *main, t_cmd *redir)
{
	if (!redir)
		return (NULL);
	if (redir && redir->type == CMD_FILEOUT)
		create_redir_out(main, redir);
	else if (redir && redir->type == CMD_FILEOUT_APPPEND)
		create_redir_append_out(main, redir);
	else if (redir && redir->type == CMD_FILEIN)
		create_redir_in(main, redir);
	else if (redir && redir->type == CMD_HEREDOC)
		create_redir_heredoc(main, redir);
	if (redir && redir->next)
		redir = create_fd_redir(main, redir->next);
	else if (redir && !redir->next)
	{
		close_unnecessary_out_fd(redir);
		close_unnecessary_in_fd(redir);
	}
	return (redir);
}

void	close_redirections(t_cmd *cmd)
{
	if (!cmd->next || (!cmd->next->out_redir && !cmd->next->in_redir))
		return ;
	// if (is_redirection(cmd->next->cmd_seq_type))
	// {
	// 	while (cmd && (cmd->out_redir || cmd->in_redir))
	// 	{
	// 		if (cmd->out_redir)
	// 		{
	// 			if (close(cmd->out_redir->fd) == -1)
	// 				free_all_and_exit2(errno, "Could not close the fd");
	// 		}
	// 		else if (cmd->in_redir)
	// 		{
	// 			if (close(cmd->in_redir->fd) == -1)
	// 				free_all_and_exit2(errno, "Could not close the fd");
	// 		}
	// 		cmd = cmd->next;
	// 	}
	// }
}

void	close_files_redirections(t_cmd *cmd)
{
	if (!cmd)
		return ;
	if (cmd->in_redir && cmd->in_redir->fd > 0)
		close(cmd->in_redir->fd);
	if (cmd->out_redir && cmd->out_redir->fd > 0)
		close(cmd->out_redir->fd);
}

void	redirect_input(t_cmd *cmd)
{
	if (!cmd || !cmd->in_redir)
		return ;

	if (dup2(cmd->in_redir->fd, STDIN_FILENO) == -1)
		free_all_and_exit2(errno, "Could not redirect input");
	if (cmd->type == CMD)
	{
		if (close(cmd->in_redir->fd) == -1)
			free_all_and_exit2(errno, "Could not close the fd");
	}
}

void	redirect_output(t_cmd *cmd)
{
	if (!cmd || !cmd->out_redir)
		return ;

	if (dup2(cmd->out_redir->fd, STDOUT_FILENO) == -1)
		free_all_and_exit2(errno, "Could not redirect output");
	if (cmd->type == CMD)
	{
		if (close(cmd->out_redir->fd) == -1)
			free_all_and_exit2(errno, "Could not close the fd");
	}
}
