/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   continuation.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/07/24 09:39:12 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	is_valid_line(char *line)
{
	t_process	*proc;

	proc = get_process();
	if (line == NULL)
	{
		proc->errnum = 258;
		return (ft_printf("syntax error: unexpected end of file.\n"), false);
	}
	return (true);
}

void	write_lines(t_redirect *redir)
{
	int32_t		i;
	char		*line;
	t_process	*proc;

	proc = get_process();
	proc->errnum = 0;
	line = readline("> ");
	while (is_valid_line(line))
	{
		i = 0;
		while (line[i] && line[i] == ' ')
			i++;
		if (line[i])
		{
			if (write(redir->fd, line, ft_strlen(line)) == -1)
				free_all_and_exit2(errno, "malloc error");
			free(line);
			break ;
		}
		free(line);
		line = readline("> ");
	}
	close(redir->fd);
}

int32_t	write_non_empty_continuation(void)
{
	pid_t		pid;
	t_process	*proc;

	proc = get_process();
	proc->execution = EXEC_CONTINUATION;
	setup_child_realine_signal_handlers();
	proc->errnum = 0;
	pid = ft_fork();
	if (pid == 0)
	{
		proc = get_process();
		write_lines(proc->continuation);
		free_exit_no_perr(proc->errnum);
	}
	reset_signal_handlers();
	proc->errnum = ft_waitpid(pid);
	if (proc->errnum == 2)
		proc->errnum = 258;
	close(proc->continuation->fd);
	proc->execution = EXEC_END;
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
