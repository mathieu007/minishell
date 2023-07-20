#include "minishell.h"

void	write_here_doc_lines(t_cmd *main, const char *delimiter)
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
			break ;
		}
		write(main->in_redir->fd, line, ft_strlen(line));
		write(main->in_redir->fd, "\n", 1);
		free(line);
		line = readline("> ");
	}	
}

int32_t	write_here_document(const char *delimiter, t_cmd *main, t_cmd *redir)
{
	pid_t		pid;
	t_process	*proc;

	proc = get_process();
	
	create_redir_heredoc(main, redir);	
	proc->in_here_doc = true;
	pid = ft_fork();
	if (pid == 0)
	{		
		proc = get_process();		
		setup_child_realine_signal_handlers();
		write_here_doc_lines(main, delimiter);
		close_files_redirections(main);
		free_all_and_exit(0);
	}
	proc->errnum = ft_waitpid(pid);
	proc->in_here_doc = true;
	close_files_redirections(main);
	return (proc->errnum);
}
