#include "minishell.h"

static void	open_temp_file(t_cmd *cmd)
{
	char		*temp;
	char		*name;
	t_redirect	*redir;
	int32_t		flags;

	flags = O_RDONLY | O_CREAT | O_TRUNC;
	redir = cmd->in_redir;
	temp = redir->file;
	name = ft_strchrlast(redir->file, '/');
	name = ft_strjoinfree(name, "2159365424020");
	free(temp);
	if (!name)
		free_all_and_exit2(errno, "Failed to create file name");
	redir->file = ft_strjoin(get_cwd(cmd), name + 1);
	if (!redir->file)
		free_all_and_exit2(errno, "Failed to find appropriate file location");
	redir->fd = open(redir->file, flags, 0777);
	if (redir->fd == -1)
		free_all_and_exit2(errno, "Failed to open fd");
	redir->fd_is_temp = true;
}

int32_t	open_redir_heredoc(t_cmd *cmd)
{
	int32_t		flags;
	t_redirect	*redir;
	char		*f_name;

	flags = O_RDONLY;
	f_name = ft_strjoin(get_cwd(cmd), cmd->name);
	if (!f_name)
		free_all_and_exit2(errno, "malloc error");
	if (!cmd->in_redir)
		cmd->in_redir = ft_calloc(1, sizeof(t_redirect));
	if (cmd->in_redir == NULL)
		free_all_and_exit2(errno, "Failed to create t_redirect obj");
	redir = cmd->in_redir;
	redir->file = f_name;
	redir->input_file = ft_strdup(redir->file);
	redir->fd = open(redir->file, flags, 0777);
	if (redir->fd == -1)
		open_temp_file(cmd);
	if (redir->fd == -1)
		write_err2(1, cmd->name, ": No such file or directory\n");
	return (redir->fd);
}

int32_t	open_in_redir_fd(t_cmd *cmd)
{
	int32_t		flags;
	t_redirect	*redir;
	char		*f_name;

	flags = O_RDONLY;
	f_name = ft_strjoin(get_cwd(cmd), cmd->name);
	if (!f_name)
		free_all_and_exit2(errno, "malloc error");
	if (!cmd->in_redir)
		cmd->in_redir = ft_calloc(1, sizeof(t_redirect));
	if (cmd->in_redir == NULL)
		free_all_and_exit2(errno, "Failed to create t_redirect obj");
	redir = cmd->in_redir;
	redir->file = f_name;
	redir->input_file = ft_strdup(redir->file);
	redir->fd = open(redir->file, flags, 0777);
	if (redir->fd == -1)
		write_err2(1, cmd->name, ": No such file or directory\n");
	return (redir->fd);
}
