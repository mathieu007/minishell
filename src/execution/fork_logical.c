/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_logical.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: math <math@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/08/01 19:04:48 by math             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int32_t	exec_logical_or(t_cmd *or_cmd)
{
	t_process	*proc;
	t_cmd		*child;
	t_cmd_seq	type;

	proc = get_process();
	if (proc->errnum == 0)
		proc->errnum = 1;
	while (or_cmd)
	{
		child = or_cmd->child;
		type = child->type;
		if (proc->signal == SIGINT)
			return (proc->errnum);
		if (proc->errnum > 0 && (type == CMD_PARENTHESES || type == CMD))
			proc->errnum = dispatch_command(child, false);
		else if (proc->errnum > 0)
			proc->errnum = dispatch_command(child, false);
		else if (proc->errnum == 0 && child->next && child->type != CMD_PIPE)
			proc->errnum = dispatch_command(child->next, false);
		or_cmd = or_cmd->next;
	}
	return (proc->errnum);
}

int32_t	exec_logical_and(t_cmd *and_cmd)
{
	t_process	*proc;

	proc = get_process();
	proc->errnum = 0;
	while (and_cmd && proc->errnum == 0)
	{
		proc->errnum = dispatch_command(and_cmd->child, false);
		if (proc->errnum > 0)
			return (proc->errnum);
		and_cmd = and_cmd->next;
	}
	return (proc->errnum);
}
