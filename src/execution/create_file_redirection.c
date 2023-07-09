#include "minishell.h"

bool	is_redirection(t_cmd_seq seq)
{
	return (seq && (seq == CMD_FILEIN || seq == CMD_FILEOUT
			|| seq == CMD_FILEOUT_APPPEND || seq == CMD_HEREDOC));
}

inline bool	is_token_redirection(t_token_type type)
{
	return (type && (type == TK_GREAT || type == TK_GREATGREAT
			|| type == TK_LESS || type == TK_LESSLESS));
}

void	copy_redirection(t_redirect *dest, t_redirect *src)
{
	dest->fd = src->fd;
	dest->fd_is_temp = src->fd_is_temp;
	dest->file = free_ptr(dest->file);
	dest->input_file = free_ptr(dest->input_file);
	dest->file = ft_strdup(src->file);
	dest->input_file = ft_strdup(src->input_file);
}

t_cmd	*create_redir_out(t_cmd *main, t_cmd *redir)
{
	open_out_redir_fd(redir);
	main->out_redir = free_t_redirect(main->out_redir);
	main->out_redir = new_redirect();
	copy_redirection(main->out_redir, redir->out_redir);
	return (redir);
}

t_cmd	*create_redir_append_out(t_cmd *main, t_cmd *redir)
{
	open_out_append_redir_fd(redir);
	main->out_redir = free_t_redirect(main->out_redir);
	main->out_redir = new_redirect();
	copy_redirection(main->out_redir, redir->out_redir);
	return (redir);
}

t_cmd	*create_redir_in(t_cmd *main, t_cmd *redir)
{
	open_in_redir_fd(redir);
	main->in_redir = free_t_redirect(main->in_redir);
	main->in_redir = new_redirect();
	copy_redirection(main->in_redir, redir->in_redir);
	return (redir);
}

int32_t	write_here_document(const char *delimiter, t_cmd *main)
{
	char		*line;
	pid_t		pid;
	int32_t		status;
	int32_t		delimiter_len;
	t_process	*proc;

	delimiter_len = strlen(delimiter);
	proc = get_process();
	pid = fork();
	if (pid == -1)
		free_all_and_exit2(errno, "fork error");
	else if (pid == 0)
	{
		proc = get_process();
		proc->is_here_doc = true;
		line = readline("> ");
		while (line)
		{
			if (ft_strncmp(line, delimiter, delimiter_len) == 0)
			{
				free(line);
				close_files_redirections(main);
				exit(0);
			}
			write(main->in_redir->fd, line, ft_strlen(line));
			free(line);
			line = readline("> ");
		}
		close_files_redirections(main);
		exit(0);
	}
	close_files_redirections(main);
	if (waitpid(pid, &status, 0) == -1)
		free_all_and_exit2(errno, "waitpid error");
	if (WIFEXITED(status))
		proc->errnum = WEXITSTATUS(status);
	return (proc->errnum);
}

t_cmd	*create_redir_heredoc(t_cmd *main, t_cmd *redir)
{
	open_redir_heredoc(redir);
	main->in_redir = free_t_redirect(main->in_redir);
	main->in_redir = new_redirect();
	main->in_redir->is_here_doc = true;
	copy_redirection(main->in_redir, redir->in_redir);
	return (redir);
}

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

void	close_unused_open_last_redirs(t_cmd *main, t_cmd *redir)
{
	close_out_fds(redir);
	close_in_fds(redir);
	if (main->in_redir && main->in_redir->file)
		open_in_redir_fd(main);
	if (main->out_redir && main->out_redir->file && main->out_redir->is_append)
		open_out_append_redir_fd(main);
	else if (main->out_redir && main->out_redir->file)
		open_out_redir_fd(main);
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
	{
		close_files_redirections(main);
		// free_all_and_exit(proc->errnum);
	}
	if (redir->next)
		build_redir_environement(main, redir->next);
}

t_cmd	*create_fd_redir(t_cmd *main, t_cmd *redir)
{
	t_process	*proc;

	proc = get_process();
	if (!redir)
		return (NULL);
	if (!main->redir_processed)
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
	if (redir && !redir->next)
		close_unused_open_last_redirs(main, redir);
	redir = create_fd_redir(main, redir->next);
	return (redir);
}

void	close_files_redirections(t_cmd *cmd)
{
	if (!cmd)
		return ;
	if (cmd->in_redir && cmd->in_redir->fd > 0)
	{
		close(cmd->in_redir->fd);
		cmd->in_redir->fd = -1;
	}
	if (cmd->out_redir && cmd->out_redir->fd > 0)
	{
		close(cmd->out_redir->fd);
		cmd->out_redir->fd = -1;
	}
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
		cmd->in_redir->fd = -1;
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
		cmd->out_redir->fd = -1;
	}
}
