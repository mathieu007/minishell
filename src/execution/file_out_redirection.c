#include "minishell.h"

bool	is_a_directory(char *path_to_dir)
{
	DIR	*dir;

	dir = opendir(path_to_dir);
	if (dir)
	{
		closedir(dir);
		return (true);
	}
	return (false);
}

int32_t	open_out_redir_fd(t_cmd *cmd)
{
	int32_t		out_flags;
	t_redirect	*redir;
	char		*f_name;

	out_flags = O_RDWR | O_CREAT | O_TRUNC | O_CLOEXEC;
	f_name = ft_strjoin(get_cwd(cmd), cmd->name);
	if (is_a_directory(f_name))
		return (free(f_name), free_exit_no_perr2(1, cmd->name,
				": Is a directory"), -1);
	if (!cmd->out_redir)
		cmd->out_redir = ft_calloc(1, sizeof(t_redirect));
	if (cmd->out_redir == NULL)
		free_all_and_exit2(errno, "Failed to create t_redirect obj");
	redir = cmd->out_redir;
	redir->file = f_name;
	redir->fd = open(redir->file, out_flags, 0777);
	if (redir->fd == -1)
		free_all_and_exit2(errno, "Failed to open fd");
	return (redir->fd);
}

int32_t	open_out_append_redir_fd(t_cmd *cmd)
{
	int32_t		out_flags;
	t_redirect	*redir;
	char		*f_name;

	out_flags = O_RDWR | O_CREAT | O_APPEND | O_CLOEXEC;
	f_name = ft_strjoin(get_cwd(cmd), cmd->name);
	if (is_a_directory(f_name))
		return (free(f_name), free_exit_no_perr2(1, cmd->name,
				": Is a directory"), -1);
	if (!cmd->out_redir)
		cmd->out_redir = ft_calloc(1, sizeof(t_redirect));
	if (cmd->out_redir == NULL)
		free_all_and_exit2(errno, "Failed to create t_redirect obj");
	redir = cmd->out_redir;
	redir->file = f_name;
	redir->fd = open(redir->file, out_flags, 0777);
	if (redir->fd == -1)
		free_all_and_exit2(errno, "Failed to open fd");
	return (redir->fd);
}
