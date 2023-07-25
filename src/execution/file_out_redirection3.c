/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_out_redirection3.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/07/17 09:44:14 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*build_output_file_name2(t_cmd *cmd)
{
	char	*f_name;

	if (!cmd->out_redir || !cmd->out_redir->file)
		f_name = ft_strjoinfree(get_cwd_with_backslash(), cmd->name);
	else
		f_name = ft_strdup(cmd->out_redir->file);
	return (f_name);
}

int32_t	handle_directory_case(char *f_name)
{
	t_process	*proc;

	proc = get_process();
	proc->errnum = 1;
	free(f_name);
	return (-1);
}

void	set_out_redir_fd_and_flags(t_cmd *cmd, char *f_name, int32_t out_flags)
{
	t_redirect	*redir;

	redir = cmd->out_redir;
	redir->is_append = false;
	redir->file = free_ptr(redir->file);
	redir->file = f_name;
	redir->fd = open(redir->file, out_flags, 0777);
	redir->flags = out_flags;
	if (redir->fd == -1)
		write_err2(1, redir->file, ": Failed to open fd\n");
}

int32_t	open_out_redir_fd(t_cmd *cmd)
{
	int32_t	out_flags;
	char	*f_name;

	out_flags = O_RDWR | O_CREAT | O_TRUNC | O_CLOEXEC;
	f_name = build_output_file_name2(cmd);
	if (is_a_directory(f_name))
		return (handle_directory_case(f_name));
	if (!cmd->out_redir)
		cmd->out_redir = ft_calloc(1, sizeof(t_redirect));
	if (cmd->out_redir == NULL)
	{
		free(f_name);
		free_all_and_exit2(errno, "Failed to create t_redirect obj");
	}
	set_out_redir_fd_and_flags(cmd, f_name, out_flags);
	return (cmd->out_redir->fd);
}

// int32_t	open_out_redir_fd(t_cmd *cmd)
// {
// 	int32_t		out_flags;
// 	t_redirect	*redir;
// 	char		*f_name;
// 	t_process	*proc;

// 	out_flags = O_RDWR | O_CREAT | O_TRUNC | O_CLOEXEC;
// 	if (!cmd->out_redir || !cmd->out_redir->file)
// 		f_name = ft_strjoinfree(get_cwd_with_backslash(), cmd->name);
// 	else
// 		f_name = ft_strdup(cmd->out_redir->file);
// 	if (is_a_directory(f_name))
// 	{
// 		proc = get_process();
// 		proc->errnum = 1;
// 		return (free(f_name), -1);
// 	}
// 	if (!cmd->out_redir)
// 		cmd->out_redir = ft_calloc(1, sizeof(t_redirect));
// 	if (cmd->out_redir == NULL)
// 	{
// 		free(f_name);
// 		free_all_and_exit2(errno, "Failed to create t_redirect obj");
// 	}
// 	redir = cmd->out_redir;
// 	redir->is_append = false;
// 	redir->file = free_ptr(redir->file);
// 	redir->file = f_name;
// 	redir->fd = open(redir->file, out_flags, 0777);
// 	redir->flags = out_flags;
// 	if (redir->fd == -1)
// 		write_err2(1, redir->file, ": Failed to open fd\n");
// 	return (redir->fd);
// }
