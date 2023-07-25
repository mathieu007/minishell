/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_in_redirection2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/07/25 09:47:08 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_input_file_name(t_cmd *cmd)
{
	char		*f_name;
	t_process	*proc;

	proc = get_process();
	if (!cmd->in_redir || !cmd->in_redir->file)
		f_name = get_full_path(cmd->name);
	else
		f_name = ft_strdup(cmd->in_redir->file);
	if (proc->errnum)
		return (NULL);
	if (!f_name)
		free_all_and_exit2(errno, "malloc error");
	return (f_name);
}

int32_t	handle_nonexistent_file(char *f_name, char *cmd_name)
{
	write_err2(1, cmd_name, ": No such file or directory\n");
	free(f_name);
	return (-1);
}

void	set_in_redir_fd_and_flags(t_cmd *cmd, char *f_name, int32_t flags)
{
	t_redirect	*redir;

	redir = cmd->in_redir;
	redir->file = free_ptr(redir->file);
	redir->file = f_name;
	redir->input_file = ft_strdup(redir->file);
	redir->fd = open(redir->file, flags, 0777);
	redir->flags = flags;
	if (redir->fd == -1)
		handle_nonexistent_file(redir->file, cmd->name);
}

int32_t	open_in_redir_fd(t_cmd *cmd)
{
	int32_t	flags;
	char	*f_name;

	flags = O_RDONLY;
	f_name = get_input_file_name(cmd);
	if (!cmd->in_redir)
		cmd->in_redir = ft_calloc(1, sizeof(t_redirect));
	if (cmd->in_redir == NULL)
		free_all_and_exit2(errno, "Failed to create t_redirect obj");
	if (!f_name)
		return (-1);
	set_in_redir_fd_and_flags(cmd, f_name, flags);
	return (cmd->in_redir->fd);
}

// int32_t	open_in_redir_fd(t_cmd *cmd)
// {
// 	int32_t		flags;
// 	t_redirect	*redir;
// 	char		*f_name;
// 	t_process	*proc;

// 	proc = get_process();
// 	flags = O_RDONLY;
// 	if (!cmd->in_redir || !cmd->in_redir->file)
// 		f_name = get_full_path(cmd->name);
// 	else
// 		f_name = ft_strdup(cmd->in_redir->file);
// 	if (proc->errnum)
// 		return (-1);
// 	if (!f_name)
// 		free_all_and_exit2(errno, "malloc error");
// 	if (!cmd->in_redir)
// 		cmd->in_redir = ft_calloc(1, sizeof(t_redirect));
// 	if (cmd->in_redir == NULL)
// 		free_all_and_exit2(errno, "Failed to create t_redirect obj");
// 	redir = cmd->in_redir;
// 	redir->file = free_ptr(redir->file);
// 	redir->file = f_name;
// 	redir->input_file = ft_strdup(redir->file);
// 	redir->fd = open(redir->file, flags, 0777);
// 	redir->flags = flags;
// 	if (redir->fd == -1)
// 		write_err2(1, cmd->name, ": No such file or directory\n");
// 	return (redir->fd);
// }
