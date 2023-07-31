/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_subshell.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/07/31 15:22:25 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int32_t	execve_subshell(t_cmd *cmd)
{
	t_process	*proc;
	char		**subshell_args;
	char		**env;

	proc = get_process();
	env = get_env();
	subshell_args = malloc(3 * sizeof(char *));
	subshell_args[2] = NULL;
	subshell_args[1] = cmd->token->str;
	subshell_args[0] = "minishell";
	proc->full_program_name = get_full_path("./bin/minishell");
	if (execve(proc->full_program_name, subshell_args, env) == -1)
	{
		free_2d_char_array(env);
		free(subshell_args);
		free_all_and_exit2(errno, "execve subshell error");
	}
	return (errno);
}

int32_t	exec_subshell(t_cmd *cmd)
{
	pid_t		pid;
	t_process	*proc;

	proc = get_process();
	proc->errnum = build_cmd(cmd);
	if (has_error())
		return (proc->errnum);
	proc->errnum = 0;
	setup_child_signal_handlers(cmd);
	pid = ft_fork();
	if (pid == 0)
	{
		file_redirection(cmd, true);
		close_files_redirections(cmd);
		proc->errnum = execve_subshell(cmd);
		free_all_and_exit(proc->errnum);
	}
	reset_signal_handlers();
	proc->errnum = ft_waitpid(pid);
	return (proc->errnum);
}
