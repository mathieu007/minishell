/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   continuation.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/07/17 09:44:14 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	pid = ft_fork();
	if (pid == 0)
	{
		proc = get_process();
		setup_child_realine_signal_handlers();
		write_delimiter_line(redir, delimiter);
		close(redir->fd);
		free_t_redirect(redir);
		free_all_and_exit(0);
	}
	proc->errnum = ft_waitpid(pid);
	proc->in_continuation = false;
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
	pid = ft_fork();
	if (pid == 0)
	{
		setup_child_realine_signal_handlers();
		proc = get_process();
		write_line(redir);
		free_t_redirect(redir);
		free_all_and_exit(0);
	}
	proc->errnum = ft_waitpid(pid);
	proc->in_continuation = false;
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
