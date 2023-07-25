/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: math <math@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/07/24 18:41:15 by math             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	is_valid_line(char *line, const char *delimiter,
		size_t delimiter_len)
{
	if (line == NULL)
	{
		ft_printf("warning: here-document delimited by"
			" end-of-file (wanted `%s')\n", delimiter);
		return (false);
	}
	if (ft_strncmp(line, delimiter, delimiter_len) == 0
		&& ft_strlen(line) == delimiter_len)
	{
		free(line);
		return (false);
	}
	return (true);
}

void	write_here_doc_lines(t_cmd *main, const char *delimiter)
{
	char		*line;
	size_t		delimiter_len;
	t_process	*proc;

	proc = get_process();
	delimiter_len = ft_strlen(delimiter);
	line = readline("> ");
	while (is_valid_line(line, delimiter, delimiter_len) && proc->signal == 0)
	{
		write(main->in_redir->fd, line, ft_strlen(line));
		write(main->in_redir->fd, "\n", 1);
		line = free_ptr(line);
		line = readline("> ");
	}
	line = free_ptr(line);
}

int32_t	write_here_document(const char *delimiter, t_cmd *main, t_cmd *redir)
{
	pid_t		pid;
	t_process	*proc;

	proc = get_process();
	create_redir_heredoc(main, redir);
	proc->execution = EXEC_CONTINUATION;
	setup_child_realine_signal_handlers();
	pid = ft_fork();
	if (pid == 0)
	{
		write_here_doc_lines(main, delimiter);
		close_files_redirections(main);
		free_all_and_exit(0);
	}
	reset_signal_handlers();
	proc->errnum = ft_waitpid(pid);
	close_files_redirections(main);
	proc->execution = EXEC_END;
	return (proc->errnum);
}
