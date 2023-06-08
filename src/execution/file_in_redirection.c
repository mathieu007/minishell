#include "minishell.h"

static void	open_temp_file(t_cmd *cmd)
{
	char		*temp;
	char		*name;
	t_redirect	*redir;
	int32_t		flags;

	redir = cmd->out_redir;
	temp = redir->file;
	name = ft_strchrlast(redir->file, '/');
	name = ft_strjoinfree(name, "2159365424020");
	free(temp);
	if (!name)
	{
		free_all_and_exit(EXIT_FAILURE);
	}
	redir->file = ft_strjoin(get_cwd(cmd), name + 1);
	if (!redir->file)
		free_all_and_exit(EXIT_FAILURE);
	flags = O_RDONLY | O_CREAT | O_TRUNC;
	redir->fd = open(redir->file, flags, 0777);
	if (redir->fd == -1)
		free_all_and_exit(EXIT_FAILURE);
	redir->fd_is_temp = true;
}

int32_t	open_in_redir_fd(t_cmd *cmd)
{
	int32_t		flags;
	t_redirect	*redir;

	cmd->out_redir = ft_calloc(1, sizeof(t_redirect));
	if (cmd->out_redir == NULL)
		return (-1);
	redir = cmd->out_redir;
	redir->file = cmd->name;
	redir->input_file = redir->file;
	flags = O_RDONLY | O_CLOEXEC;
	redir->fd = open(redir->file, flags, 0777);
	if (redir->fd == -1)
		open_temp_file(cmd);
	if (redir->fd == -1)
	{
		write_err3(2, strerror(errno), " ", redir->input_file);
		free_all_and_exit(EXIT_FAILURE);
	}
	return (redir->fd);
}
