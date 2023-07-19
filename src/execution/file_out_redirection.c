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
	t_process	*proc;

	out_flags = O_RDWR | O_CREAT | O_TRUNC | O_CLOEXEC;
	if (!cmd->out_redir || !cmd->out_redir->file)
		f_name = ft_strjoinfree(get_cwd_with_backslash(), cmd->name);
	else
		f_name = ft_strdup(cmd->out_redir->file);
	if (is_a_directory(f_name))
	{
		proc = get_process();
		proc->errnum = 1;
		return (free(f_name), -1);
	}
	if (!cmd->out_redir)
		cmd->out_redir = ft_calloc(1, sizeof(t_redirect));
	if (cmd->out_redir == NULL)
	{
		free(f_name);
		free_all_and_exit2(errno, "Failed to create t_redirect obj");
	}
	redir = cmd->out_redir;
	redir->is_append = false;
	redir->file = free_ptr(redir->file);
	redir->file = f_name;
	redir->fd = open(redir->file, out_flags, 0777);
	redir->flags = out_flags;
	if (redir->fd == -1)
		write_err2(1, redir->file, ": Failed to open fd\n");
	return (redir->fd);
}

int32_t	open_out_append_redir_fd(t_cmd *cmd)
{
	int32_t		out_flags;
	t_redirect	*redir;
	char		*f_name;
	t_process	*proc;

	out_flags = O_RDWR | O_CREAT | O_APPEND | O_CLOEXEC;
	if (!cmd->out_redir || !cmd->out_redir->file)
		f_name = ft_strjoinfree(get_cwd_with_backslash(), cmd->name);
	else
		f_name = ft_strdup(cmd->out_redir->file);
	if (is_a_directory(f_name))
	{
		proc = get_process();
		proc->errnum = 1;
		return (free(f_name), -1);
	}
	if (!cmd->out_redir)
		cmd->out_redir = ft_calloc(1, sizeof(t_redirect));
	if (cmd->out_redir == NULL)
		free_all_and_exit2(errno, "Failed to create t_redirect obj");
	redir = cmd->out_redir;
	redir->is_append = true;
	redir->file = free_ptr(redir->file);
	redir->file = f_name;
	redir->fd = open(redir->file, out_flags, 0777);
	redir->flags = out_flags;
	if (redir->fd == -1)
		write_err2(1, cmd->name, ": Failed to open fd\n");
	return (redir->fd);
}
