/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_all2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 07:26:48 by math              #+#    #+#             */
/*   Updated: 2023/07/25 16:54:35 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_exit_no_perr3(int32_t status, char *msg, char *msg2)
{
	t_process	*proc;

	proc = get_process();
	write_err2(status, msg, msg2);
	proc->errnum = status;
	close_all_pipes();
	close_all_fds();
	free_all();
	rl_clear_history();
	exit(status);
}

void	free_exit_no_perr(int32_t status)
{
	t_process	*proc;

	proc = get_process();
	proc->errnum = status;
	close_all_pipes();
	close_all_fds();
	free_all();
	rl_clear_history();
	exit(status);
}
