#include "minishell.h"

int32_t	write_continuation(const char *delimiter, t_redirect *redir)
{
	char		*line;
	pid_t		pid;
	int32_t		status;
	int32_t		delimiter_len;
	t_process	*proc;

	delimiter_len = ft_strlen(delimiter);
	proc = get_process();
	pid = fork();
	if (pid == -1)
		free_all_and_exit2(errno, "fork error");
	else if (pid == 0)
	{
		proc = get_process();
		line = "";
		while (line)
		{
			line = readline("> ");
			if (ft_strncmp(line, delimiter, delimiter_len) == 0)
			{
				free(line);
				close(redir->fd);
				exit(0);
			}
			write(redir->fd, line, ft_strlen(line));
			free(line);
		}
		close(redir->fd);
		exit(0);
	}
	if (waitpid(pid, &status, 0) == -1)
		free_all_and_exit2(errno, "waitpid error");
	if (WIFEXITED(status))
		proc->errnum = WEXITSTATUS(status);
	return (proc->errnum);
}

int32_t	write_non_empty_continuation(t_redirect *redir)
{
	char		*line;
	pid_t		pid;
	int32_t		status;
	t_process	*proc;
	int32_t		i;

	proc = get_process();
	pid = fork();
	if (pid == -1)
		free_all_and_exit2(errno, "fork error");
	else if (pid == 0)
	{
		proc = get_process();
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
				exit(0);
			}
			free(line);
		}
		close(redir->fd);
		exit(0);
	}
	if (waitpid(pid, &status, 0) == -1)
		free_all_and_exit2(errno, "waitpid error");
	if (WIFEXITED(status))
		proc->errnum = WEXITSTATUS(status);
	return (proc->errnum);
}

t_redirect	*open_continuation(void)
{
	t_redirect	*redir;
	char		*f_name;
	char		*tmp_dir;

	tmp_dir = get_temp_dir();
	f_name = ft_strjoinfree(tmp_dir, "temp_continuation");
	if (!f_name)
		free_all_and_exit2(errno, "malloc error");
	redir = ft_calloc(1, sizeof(t_redirect));
	if (!redir)
		free_all_and_exit2(errno, "Failed to create t_redirect obj");
	redir->file = f_name;
	create_temp_file(redir);
	if (redir->fd == -1)
		write_err2(errno, f_name,
				": Unable to create temporary file or directory\n");
	return (redir);
}

void	exec_delimiter_continuation(char *delimiter, t_token *parent)
{
	t_redirect	*redir;
	static char	data[128];
	char		*str;
	size_t		size;

	data[0] = '\0';
	redir = open_continuation();
	write_continuation(delimiter, redir);
	size = 1;
	str = " ";
	while (size > 0)
	{
		ft_strjoinfree(str, &data[0]);
		size = read(redir->fd, &data[0], 127);
		if (size == (size_t)-1)
			free_all_and_exit2(errno, "read error");
		data[size] = '\0';
	}
	parent->str = ft_strjoinfree(parent->str, str);
	close(redir->fd);
	free_t_redirect(redir);
}

void	exec_continuation(t_token *parent)
{
	t_redirect	*redir;
	static char	data[128];
	char		*str;
	size_t		size;

	data[0] = '\0';
	redir = open_continuation();
	write_non_empty_continuation(redir);
	size = 1;
	str = " ";
	while (size > 0)
	{
		ft_strjoinfree(str, &data[0]);
		size = read(redir->fd, &data[0], 127);
		if (size == (size_t)-1)
			free_all_and_exit2(errno, "read error");
		data[size] = '\0';
	}
	parent->str = ft_strjoinfree(parent->str, str);
	close(redir->fd);
	free_t_redirect(redir);
}
