#include "minishell.h"

int32_t	open_out_here_doc_fd(t_cmd *cmd)
{
	int32_t		out_flags;
	t_redirect	*redir;
	char		*f_name;

	out_flags = O_WRONLY | O_CREAT | O_APPEND;
	f_name = cmd->name;
	cmd->out_redir = ft_calloc(1, sizeof(t_redirect));
	if (cmd->out_redir == NULL)
		free_all_and_exit2(errno, "Failed to create t_redirect obj");
	redir = cmd->out_redir;
	redir->file = ft_strdup(f_name);	
	redir->fd = open(redir->file, out_flags, 0777);
	if (redir->fd == -1)
		free_all_and_exit2(errno, "Failed to open fd");
	return (redir->fd);
}

int32_t	open_out_redir_fd(t_cmd *cmd)
{
	int32_t		out_flags;
	t_redirect	*redir;
	char		*f_name;

	out_flags = O_RDWR | O_CREAT | O_TRUNC;
	f_name = cmd->name;
	cmd->out_redir = ft_calloc(1, sizeof(t_redirect));
	if (cmd->out_redir == NULL)
		free_all_and_exit2(errno, "Failed to create t_redirect obj");
	redir = cmd->out_redir;
	redir->file = ft_strdup(f_name);	
	redir->fd = open(redir->file, out_flags, 0777);
	if (redir->fd == -1)
		free_all_and_exit2(errno, "Failed to open fd");
	return (redir->fd);
}

int32_t	open_out_append_redir_fd(t_cmd *cmd)
{
	int32_t		out_flags;
	t_redirect	*redir;

	cmd->out_redir = ft_calloc(1, sizeof(t_redirect));
	if (cmd->out_redir == NULL)
		free_all_and_exit2(errno, "Failed to create t_redirect obj");
	redir = cmd->out_redir;
	redir->file = ft_strdup(cmd->name);
	out_flags = O_RDWR | O_CREAT | O_APPEND;
	redir->fd = open(redir->file, out_flags, 0777);
	if (redir->fd == -1)
		free_all_and_exit2(errno, "Failed to open fd");
	return (redir->fd);
}
