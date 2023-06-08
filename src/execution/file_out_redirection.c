#include "minishell.h"

static void	open_temp_file(t_cmd *cmd)
{
	char		*temp;
	char		*name;
	t_redirect	*redir;

	redir = cmd->redir;
	temp = redir->file;
	name = ft_strchrlast(redir->file, '/');
	name = ft_strjoinfree(name, "2159365424020");
	free(temp);
	if (!name)
	{
		free(name);
		free_err_exit(EXIT_FAILURE);
	}
	redir->file = ft_strjoin(get_cwd(cmd), name + 1);
	if (!redir->file)
		free_all_and_exit(EXIT_FAILURE);
	redir->fd = open(redir->file, O_RDONLY | O_CREAT | O_TRUNC, 0777);
	if (redir->fd == -1)
		free_exit(EXIT_FAILURE);
	redir->fd_is_temp = true;
}

int32_t	open_out_here_doc_fd(t_cmd *cmd)
{
	int32_t		out_flags;
	t_redirect	*redir;

	cmd->redir = malloc(sizeof(t_redirect));
	if (cmd->redir == NULL)
		return (NULL);
	redir = cmd->redir;
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

	cmd->redir = malloc(sizeof(t_redirect));
	if (cmd->redir == NULL)
		return (NULL);
	redir = cmd->redir;
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

	cmd->redir = malloc(sizeof(t_redirect));
	if (cmd->redir == NULL)
		return (NULL);
	redir = cmd->redir;
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
