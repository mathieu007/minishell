#include "minishell.h"

void	write_delimiter_line(t_redirect *redir, const char *delimiter)
{
	char	*line;
	int32_t	delimiter_len;
	int32_t	len;
	int32_t	i;

	delimiter_len = ft_strlen(delimiter);
	line = "";
	if (write(redir->fd, "\n", 1) == -1)
		free_all_and_exit2(errno, "write error");
	while (line)
	{
		line = readline("> ");
		if (write(redir->fd, line, ft_strlen(line)) == -1)
			free_all_and_exit2(errno, "write error");
		i = 0;
		len = ft_strlen(line);
		while (i < len)
		{
			if (ft_strncmp(&line[i], delimiter, delimiter_len) == 0)
				return (free(line));
			i++;
		}
		if (write(redir->fd, "\n", 1) == -1)
			free_all_and_exit2(errno, "write error");
		free(line);
	}
}

int32_t	write_continuation(const char *delimiter, t_redirect *redir)
{
	pid_t		pid;
	t_process	*proc;

	proc = get_process();
	disable_ctrl_c_output();
	setup_child_signal_handlers();
	pid = ft_fork();
	if (pid == 0)
	{
		proc = get_process();
		proc->is_here_doc = true;
		write_delimiter_line(redir, delimiter);
		close(redir->fd);
		free_t_redirect(redir);
		free_all_and_exit(0);
	}
	proc->errnum = ft_waitpid(pid);
	return (proc->errnum);
}

void	write_line(t_redirect *redir)
{
	int32_t	i;
	char	*line;

	line = "";
	while (line)
	{
		line = readline("> ");
		i = 0;
		while (line[i] && line[i] == ' ')
			i++;
		if (line[i])
		{
			write(redir->fd, line, ft_strlen(line));
			free(line);
			break ;
		}
		free(line);
	}
	close(redir->fd);
}

int32_t	write_non_empty_continuation(t_redirect *redir)
{
	pid_t		pid;
	t_process	*proc;

	proc = get_process();
	disable_ctrl_c_output();
	setup_child_signal_handlers();
	pid = ft_fork();
	if (pid == 0)
	{
		proc = get_process();
		proc->is_here_doc = true;
		write_line(redir);
		free_t_redirect(redir);
		free_all_and_exit(0);
	}
	proc->errnum = ft_waitpid(pid);
	return (proc->errnum);
}

t_redirect	*open_write_continuation(void)
{
	t_redirect	*redir;
	char		*f_name;
	char		*tmp_dir;

	tmp_dir = get_temp_dir();
	f_name = ft_strjoinfree(tmp_dir, "temp_continuation");
	if (!f_name)
	{
		free(tmp_dir);
		free_all_and_exit2(errno, "malloc error");
	}
	redir = ft_calloc(1, sizeof(t_redirect));
	if (!redir)
	{
		free(tmp_dir);
		free(f_name);
		free_all_and_exit2(errno, "Failed to create t_redirect obj");
	}
	redir->file = free_ptr(redir->file);
	redir->file = f_name;
	create_temp_file(redir);
	if (redir->fd == -1)
		write_err2(errno, f_name,
				": Unable to create temporary file or directory\n");
	return (redir);
}

t_redirect	*open_read_continuation(t_redirect *redir)
{
	open_read_temp_file(redir);
	if (redir->fd == -1)
		write_err(errno, ": Unable open temporary file or directory\n");
	return (redir);
}

void	exec_delimiter_continuation(char *delimiter, t_token *parent)
{
	t_redirect	*redir;
	static char	buffer[128];
	char		*str;
	size_t		size;

	buffer[0] = '\0';
	redir = open_write_continuation();
	write_continuation(delimiter, redir);
	open_read_temp_file(redir);
	size = 1;
	str = NULL;
	while (size > 0)
	{
		size = read(redir->fd, buffer, 127);
		buffer[size] = '\0';
		str = ft_strjoinfree(str, &buffer[0]);
		if (size == (size_t)-1)
		{
			free(str);
			close(redir->fd);
			free_t_redirect(redir);
			free_all_and_exit2(errno, "read error");
		}
	}
	parent->str = ft_strjoinfree(parent->str, str);
	free(str);
	close(redir->fd);
	free_t_redirect(redir);
}

t_redirect	*exec_continuation(t_token *parent)
{
	t_redirect	*redir;
	char		buffer[128];
	char		*str;
	size_t		size;

	size = 1;
	redir = open_write_continuation();
	write_non_empty_continuation(redir);
	open_read_temp_file(redir);
	str = malloc(2);
	if (!str)
		return (free_all_and_exit2(errno, "malloc error"), NULL);
	str[0] = ' ';
	str[1] = '\0';
	while (size > 0)
	{
		size = read(redir->fd, buffer, 127);
		buffer[size] = '\0';
		str = ft_strjoinfree(str, &buffer[0]);
		if (size == (size_t)-1)
		{
			free(str);
			close(redir->fd);
			free_all_and_exit2(errno, "read error");
		}
	}
	parent->str = ft_strjoinfree(parent->str, str);
	free(str);
	close(redir->fd);
	return (redir);
}
