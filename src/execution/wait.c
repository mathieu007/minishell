/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wait.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/07/17 09:44:14 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

pid_t	ft_fork(void)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		free_all_and_exit2(errno, "fork error");
	return (pid);
}

int32_t	ft_waitpid(pid_t pid)
{
	int32_t		status;
	t_process	*proc;

	proc = get_process();
	while (true)
	{
		if (waitpid(pid, &status, 0) == -1)
		{
			if (errno == EINTR)
				continue ;
			else
				free_all_and_exit2(errno, "waitpid error");
		}
		if (WIFSIGNALED(status))
			proc->errnum = 128 + WTERMSIG(status);
		else if (WIFEXITED(status))
			proc->errnum = WEXITSTATUS(status);
		else
			free_all_and_exit2(errno, "waitpid error");
		return (proc->errnum);
	}
}

void	wait_childs(t_cmd *cmd)
{
	t_process	*proc;

	proc = get_process();
	while (cmd && cmd->pid)
	{
		proc->errnum = ft_waitpid(cmd->pid);
		cmd = cmd->next;
	}
}
