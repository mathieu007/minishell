#include "minishell.h"

void	write_here_doc_line(t_cmd *main, const char *delimiter)
{
	char	*line;
	size_t	delimiter_len;

	delimiter_len = ft_strlen(delimiter);
	line = readline("> ");
	while (line)
	{
		if (ft_strncmp(line, delimiter, delimiter_len) == 0
			&& ft_strlen(line) == delimiter_len)
		{
			free(line);
			write(main->in_redir->fd, "\n", 1);
			break ;
		}
		write(main->in_redir->fd, line, ft_strlen(line));
		write(main->in_redir->fd, "\n", 1);
		free(line);
		line = readline("> ");
	}
	close_files_redirections(main);
}

int32_t	write_here_document(const char *delimiter, t_cmd *main)
{
	pid_t		pid;
	t_process	*proc;

	proc = get_process();
	pid = ft_fork();
	if (pid == 0)
	{
		proc = get_process();
		proc->is_here_doc = true;
		write_here_doc_line(main, delimiter);
		free_all_and_exit(0);
	}
	proc->errnum = ft_waitpid(pid);
	return (proc->errnum);
}
