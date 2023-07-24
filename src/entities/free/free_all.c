/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_all.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 07:26:48 by math              #+#    #+#             */
/*   Updated: 2023/07/24 09:32:08 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*free_ptr(void *ptr)
{
	if (ptr)
		free(ptr);
	return (NULL);
}

void	free_all(void)
{
	t_process	*proc;

	proc = get_process();
	free_t_process(proc);
}

void	*free_all_and_exit(int32_t status)
{
	close_all_fds();
	free_all();
	exit(status);
	return (NULL);
}

void	free_all_and_exit2(int32_t status, char *msg)
{
	t_process	*proc;

	proc = get_process();
	proc->errnum = status;
	perror(msg);
	close_all_fds();
	free_all();
	exit(status);
}

void	free_exit_no_perr2(int32_t status, char *msg, char *msg2)
{
	t_process	*proc;

	proc = get_process();
	write_err2(status, msg, msg2);
	proc->errnum = status;
	close_all_fds();
	free_all();
	exit(status);
}

void	free_exit_no_perr(int32_t status)
{
	t_process	*proc;

	proc = get_process();
	proc->errnum = status;
	close_all_fds();
	free_all();
	exit(status);
}