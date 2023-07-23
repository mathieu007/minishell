/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   continuation_delimiter.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: math <math@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/07/23 10:20:44 by math             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	is_not_end_line(char *line, const char *delimiter)
{
	t_process	*proc;

	proc = get_process();
	if (line == NULL)
	{
		proc->errnum = 2;
		ft_printf("unexpected EOF while looking for matching `%s'\n",
				delimiter);
		ft_printf("syntax error: unexpected end of file\n");
		return (false);
	}
	return (true);
}

void	write_delimiter_lines(t_redirect *redir, const char *delimiter)
{
	char	*line;
	int32_t	i;

	line = readline("> ");
	while (is_not_end_line(line, delimiter))
	{
		if (write(redir->fd, line, ft_strlen(line)) == -1)
			free_all_and_exit2(errno, "write error");
		i = 0;
		while (line[i])
		{
			if (ft_strcontains(&line[i], delimiter))
				return (free(line));
			i++;
		}
		if (write(redir->fd, "\n", 1) == -1)
			free_all_and_exit2(errno, "write error");
		free_ptr(line);
		line = readline("> ");
	}
	free_ptr(line);
}

t_redirect	*open_read_continuation(t_redirect *redir)
{
	open_read_temp_file(redir);
	if (redir->fd == -1)
		write_err(errno, ": Unable open temporary file or directory\n");
	return (redir);
}

void	free_continuation(void)
{
	t_process	*proc;

	proc = get_process();
	unlink_files_redirections(proc->continuation);
	close_all_fds();
	free_t_redirect(proc->continuation);
}

void	exec_delimiter_continuation(char *delimiter, t_token *parent)
{
	static char	buffer[128];
	char		*str;
	size_t		size;
	t_process	*proc;

	buffer[0] = '\0';
	proc = get_process();
	proc->continuation = open_write_continuation();
	write_delimiter_continuation(delimiter, proc->continuation);
	if (proc->errnum > 0)
		return (free_continuation());
	open_read_temp_file(proc->continuation);
	size = 1;
	str = NULL;
	while (size > 0)
	{
		size = read(proc->continuation->fd, buffer, 127);
		if (size == (size_t)-1)
			return (free(str), free_all_and_exit2(errno, "read error"));
		buffer[size] = '\0';
		str = ft_strjoinfree(str, &buffer[0]);
	}
	parent->str = ft_strjoinfree(parent->str, str);
	free(str);
	free_continuation();
}

void	exec_continuation(t_token *parent)
{
	t_process	*proc;
	char		buffer[128];
	char		*str;
	size_t		size;

	buffer[0] = '\0';
	proc = get_process();
	proc->continuation = open_write_continuation();
	write_non_empty_continuation();
	if (proc->errnum > 0)
		return (free_continuation());
	open_read_temp_file(proc->continuation);
	str = malloc(2);
	if (!str)
		return (free_all_and_exit2(errno, "malloc error"));
	str[0] = ' ';
	str[1] = '\0';
	size = 1;
	while (size > 0)
	{
		size = read(proc->continuation->fd, buffer, 127);
		if (size == (size_t)-1)
			return (free(str), free_all_and_exit2(errno, "read error"));
		buffer[size] = '\0';
		str = ft_strjoinfree(str, &buffer[0]);
	}
	parent->str = ft_strjoinfree(parent->str, str);
	free_ptr(str);
	free_continuation();
}

int32_t	write_delimiter_continuation(const char *delimiter, t_redirect *redir)
{
	pid_t		pid;
	t_process	*proc;

	proc = get_process();
	proc->execution = EXEC_CONTINUATION;
	proc->errnum = 0;
	setup_child_realine_signal_handlers();
	pid = ft_fork();
	if (pid == 0)
	{
		proc = get_process();
		write_delimiter_lines(redir, delimiter);
		free_all_and_exit(proc->errnum);
	}
	reset_signal_handlers();
	proc->errnum = ft_waitpid(pid);
	close(redir->fd);
	proc->execution = EXEC_END;
	return (proc->errnum);
}
