#include "minishell.h"

int32_t	open_out_here_doc_fd(t_cmd *cmd)
{
	int32_t		out_flags;
	t_redirect	*redir;

	cmd->out_redir = ft_calloc(1, sizeof(t_redirect));
	if (cmd->out_redir == NULL)
		return (-1);
	redir = cmd->out_redir;
	redir->file = cmd->name;
	out_flags = O_WRONLY | O_CREAT | O_CLOEXEC | O_APPEND;
	redir->fd = open(redir->file, out_flags, 0644);
	if (redir->fd == -1)
	{
		write_err3(2, strerror(errno), " ", redir->file);
		free_all_and_exit(EXIT_FAILURE);
	}
	return (redir->fd);
}

int32_t	open_out_redir_fd(t_cmd *cmd)
{
	int32_t		out_flags;
	t_redirect	*redir;

	cmd->out_redir = ft_calloc(1, sizeof(t_redirect));
	if (cmd->out_redir == NULL)
		return (-1);
	redir = cmd->out_redir;
	redir->file = cmd->name;
	out_flags = O_WRONLY | O_CREAT | O_CLOEXEC | O_TRUNC;
	redir->fd = open(redir->file, out_flags, 0644);
	if (redir->fd == -1)
	{
		write_err3(2, strerror(errno), " ", redir->file);
		free_all_and_exit(EXIT_FAILURE);
	}
	return (redir->fd);
}

int32_t	open_out_append_redir_fd(t_cmd *cmd)
{
	int32_t		out_flags;
	t_redirect	*redir;

	cmd->out_redir = ft_calloc(1, sizeof(t_redirect));
	if (cmd->out_redir == NULL)
		return (-1);
	redir = cmd->out_redir;
	redir->file = cmd->name;
	out_flags = O_WRONLY | O_CREAT | O_CLOEXEC | O_APPEND;
	redir->fd = open(redir->file, out_flags, 0644);
	if (redir->fd == -1)
	{
		write_err3(2, strerror(errno), " ", redir->file);
		free_all_and_exit(EXIT_FAILURE);
	}
	return (redir->fd);
}
