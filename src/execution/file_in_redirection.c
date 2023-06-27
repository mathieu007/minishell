#include "minishell.h"

static void	create_temp_file(t_cmd *cmd)
{
	char		*name;
	t_redirect	*redir;
	int32_t		flags;

	flags = O_RDWR | O_CREAT | O_TRUNC;
	redir = cmd->in_redir;
	name = redir->file;
	redir->fd = open(redir->file, flags, 0777);
	if (redir->fd == -1)
		free_all_and_exit2(errno, "Failed to open fd");
	redir->fd_is_temp = true;
}

static char	*get_temp_dir(t_cmd *cmd)
{
	char		*tmp_dir;

	tmp_dir = get_env_value("TMPDIR");
	if (!tmp_dir)
	tmp_dir = get_cwd(cmd);
	if (!tmp_dir)
		free_all_and_exit2(1, "An error occur while trying to get temporary dir.");
	return (tmp_dir);
}

int32_t	open_redir_heredoc(t_cmd *cmd)
{
	t_redirect	*redir;
	char		*f_name;
	char		*tmp_dir;

	tmp_dir = get_temp_dir(cmd);
	f_name = ft_strjoin(tmp_dir, "temp_here_doc");
	if (!f_name)
		free_all_and_exit2(errno, "malloc error");
	if (!cmd->in_redir)
		cmd->in_redir = ft_calloc(1, sizeof(t_redirect));
	if (!cmd->in_redir)
		free_all_and_exit2(errno, "Failed to create t_redirect obj");
	redir = cmd->in_redir;
	redir->file = f_name;
	redir->input_file = ft_strdup(redir->file);
	create_temp_file(cmd);
	if (redir->fd == -1)
		write_err2(errno, cmd->name, ": Unable to create temporary file or directory\n");
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
