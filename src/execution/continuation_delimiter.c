/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   continuation_delimiter.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: math <math@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/07/24 18:40:22 by math             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	is_not_end_line(char *line, const char *delimiter)
{
	t_process	*proc;

	proc = get_process();
	if (line == NULL)
	{
		proc->errnum = 258;
		ft_printf("unexpected EOF while looking for matching `%s'\n",
			delimiter);
		ft_printf("syntax error: unexpected end of file\n");
		return (false);
	}
	return (true);
}

void	write_delimiter_lines(t_redirect *redir, const char *delimiter)
{
	char		*line;
	int32_t		i;

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
		line = free_ptr(line);
		line = readline("> ");
	}
	line = free_ptr(line);
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
	close_all_fds();
	unlink_files_redirections(proc->continuation);
	proc->continuation = free_t_redirect(proc->continuation);
}
