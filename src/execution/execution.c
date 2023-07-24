/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: math <math@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/07/22 07:25:06 by math             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int32_t	execve_cmd(t_cmd *cmd)
{
	char	**env;

	env = get_env();
	if (execve(cmd->full_path_name, cmd->args, env) == -1)
	{
		free_2d_char_array(env);
		free_all_and_exit2(errno, "execve error");
	}
	return (errno);
}

int32_t	add_execve_func(t_cmd *cmd)
{
	cmd->func = &execve_cmd;
	return (1);
}

int32_t	exec_from_subshell_process(t_cmd *cmd)
{
	t_process	*proc;

	proc = get_process();
	file_redirection(cmd, false);
	proc->errnum = cmd->func(cmd);
	close_files_redirections(cmd);
	return (proc->errnum);
}

int32_t	exec_from_main_process(t_cmd *cmd)
{
	t_process	*proc;

	proc = get_process();
	file_redirection(cmd, false);
	proc->errnum = cmd->func(cmd);
	close_files_redirections(cmd);
	return (proc->errnum);
}

int32_t	exec_from_child_process(t_cmd *cmd)
{
	t_process	*proc;

	proc = get_process();
	file_redirection(cmd, true);
	close_files_redirections(cmd);
	proc->errnum = cmd->func(cmd);
	free_all_and_exit(proc->errnum);
	return (proc->errnum);
}
