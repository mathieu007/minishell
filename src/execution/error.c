/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/07/17 09:44:14 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

inline bool	has_error(void)
{
	t_process	*proc;

	proc = get_process();
	if (proc->errnum > 0 || proc->syntax_error)
		return (true);
	return (false);
}

/// @param stderror
/// @param msg
/// @param msg2
void	write_err2(int32_t error, char *msg, char *msg2)
{
	char		*err_msg;
	t_process	*proc;

	proc = get_process();
	err_msg = ft_strjoin(msg, msg2);
	if (!err_msg)
		free_all_and_exit2(errno, "malloc error");
	if (err_msg[ft_strlen(err_msg) - 1] != '\n')
		err_msg = ft_strjoinfree(err_msg, "\n");
	write(2, err_msg, ft_strlen(err_msg));
	proc->errnum = error;
	if (proc->last_error)
		free(proc->last_error);
	proc->last_error = err_msg;
}

void	write_err3(int32_t error, char *msg, char *msg2, char *msg3)
{
	char		*err_msg;
	t_process	*proc;

	proc = get_process();
	err_msg = ft_strjoin(msg, msg2);
	if (!err_msg)
		free_all_and_exit2(errno, "malloc error");
	err_msg = ft_strjoinfree(err_msg, msg3);
	if (!err_msg)
		free_all_and_exit2(errno, "malloc error");
	if (err_msg[ft_strlen(err_msg) - 1] != '\n')
		err_msg = ft_strjoinfree(err_msg, "\n");
	write(2, err_msg, ft_strlen(err_msg));
	proc = get_process();
	if (proc->last_error)
		free(proc->last_error);
	proc->errnum = error;
	proc->last_error = err_msg;
}

void	write_err(int32_t error, char *msg)
{
	t_process	*proc;

	proc = get_process();
	proc->errnum = error;
	if (proc->last_error)
		free(proc->last_error);
	proc->last_error = ft_strdup(msg);
	if (proc->last_error[ft_strlen(proc->last_error) - 1] != '\n')
		proc->last_error = ft_strjoinfree(proc->last_error, "\n");
	write(2, proc->last_error, ft_strlen(proc->last_error));
}

void	write_msg(int32_t std_fileno, char *msg)
{
	if (msg)
		write(std_fileno, msg, ft_strlen(msg));
}
