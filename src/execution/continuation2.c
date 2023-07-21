/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   continuation2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/07/17 09:44:14 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		if (size == (size_t) - 1)
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
		if (size == (size_t) - 1)
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
