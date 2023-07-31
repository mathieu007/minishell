/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/07/25 12:42:24 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exit_cmd(t_cmd *cmd)
{
	char		args[32];
	t_process	*proc;
	int32_t		errnum;

	proc = get_process();
	errnum = proc->last_errnum;
	if (!cmd->args || cmd->args[1] == NULL)
	{
		printf("exit\n");
		free_all_and_exit(errnum);
	}
	if (cmd->args && cmd->args[1] && !ft_isnum(cmd->args[1]))
		free_exit_no_perr3(255, cmd->args[1], ": numeric argument required");
	else if (cmd->args && cmd->args[1] && ft_isnum(cmd->args[1]) && !cmd->args[2])
	{
		printf("exit\n");
		ft_strlcpy(args, cmd->args[1], 32);
		free_exit_no_perr(ft_atoi(args) % 256);
	}
	else
	{
		ft_printf("exit: too many arguments\n");
		proc->errnum = 1;
	}
	return (proc->errnum);
}
