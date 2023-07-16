#include "minishell.h"

void	create_temp_file(t_redirect *redir)
{
	int32_t	flags;

	flags = O_WRONLY | O_CREAT | O_TRUNC;
	redir->fd = open(redir->file, flags, 0777);
	if (redir->fd == -1)
	{
		free_t_redirect(redir);
		free_all_and_exit2(errno, "Failed to open fd");
	}
	redir->fd_is_temp = true;
}

void	open_read_temp_file(t_redirect *redir)
{
	int32_t	flags;

	flags = O_RDONLY;
	redir->fd = open(redir->file, flags, 0777);
	if (redir->fd == -1)
		free_all_and_exit2(errno, "Failed to open fd");
	redir->fd_is_temp = true;
}

char	*get_temp_dir(void)
{
	char	*tmp_dir;

	tmp_dir = get_env_value("TMPDIR");
	if (!tmp_dir)
		tmp_dir = get_cwd();
	if (!tmp_dir)
		free_all_and_exit2(1,
							"An error occur while trying to get temporary dir.");
	return (tmp_dir);
}

int32_t	open_redir_heredoc(t_cmd *cmd)
{
	t_redirect	*redir;
	char		*f_name;
	char		*tmp_dir;
	int32_t		flags;

	flags = O_RDWR | O_CREAT | O_TRUNC;
	if (!cmd->in_redir || !cmd->in_redir->file)
	{
		tmp_dir = get_temp_dir();
		f_name = ft_strjoinfree(tmp_dir, "temp_here_doc");
	}
	else
		f_name = ft_strdup(cmd->in_redir->file);
	if (!f_name)
		free_all_and_exit2(errno, "malloc error");
	if (!cmd->in_redir)
		cmd->in_redir = ft_calloc(1, sizeof(t_redirect));
	if (!cmd->in_redir)
		free_all_and_exit2(errno, "Failed to create t_redirect obj");
	redir = cmd->in_redir;
	redir->file = free_ptr(redir->file);
	redir->file = f_name;
	redir->input_file = ft_strdup(redir->file);
	redir->flags = flags;
	create_temp_file(redir);
	if (redir->fd == -1)
		write_err2(errno, cmd->name,
				": Unable to create temporary file or directory\n");
	return (redir->fd);
}

int32_t	open_in_redir_fd(t_cmd *cmd)
{
	int32_t		flags;
	t_redirect	*redir;
	char		*f_name;

	flags = O_RDONLY;
	if (!cmd->in_redir || !cmd->in_redir->file)
		f_name = ft_strjoinfree(get_cwd(), cmd->name);
	else
		f_name = ft_strdup(cmd->in_redir->file);
	if (!f_name)
		free_all_and_exit2(errno, "malloc error");
	if (!cmd->in_redir)
		cmd->in_redir = ft_calloc(1, sizeof(t_redirect));
	if (cmd->in_redir == NULL)
		free_all_and_exit2(errno, "Failed to create t_redirect obj");
	redir = cmd->in_redir;
	redir->file = free_ptr(redir->file);
	redir->file = f_name;
	redir->input_file = ft_strdup(redir->file);
	redir->fd = open(redir->file, flags, 0777);
	redir->flags = flags;
	if (redir->fd == -1)
		write_err2(1, cmd->name, ": No such file or directory\n");
	return (redir->fd);
}
