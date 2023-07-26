/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_in_redirection2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: math <math@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/07/25 20:42:55 by math             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_name(t_cmd *cmd)
{
	char	*f_name;

	if (!cmd->in_redir || !cmd->in_redir->file)
		f_name = get_file_path(cmd->name);
	else
		f_name = ft_strdup(cmd->in_redir->file);
	return (f_name);
}

int32_t	open_in_redir_fd(t_cmd *cmd)
{
	t_redirect	*redir;
	char		*f_name;
	t_process	*proc;

	proc = get_process();
	f_name = get_name(cmd);
	if (proc->errnum)
		return (-1);
	if (!f_name)
		free_all_and_exit2(errno, "malloc error");
	if (!cmd->in_redir)
		cmd->in_redir = ft_calloc(1, sizeof(t_redirect));
	if (cmd->in_redir == NULL)
		free_all_and_exit2(errno, "Failed to create t_redirect obj");
	redir = cmd->in_redir;
	redir->file = free_ptr(redir->file);
	redir->file = f_name;
	redir->input_file = ft_strdup(redir->file);
	redir->fd = open(redir->file, O_RDONLY, 0777);
	redir->flags = O_RDONLY;
	if (redir->fd == -1)
		write_err2(1, cmd->name, ": No such file or directory\n");
	return (redir->fd);
}
