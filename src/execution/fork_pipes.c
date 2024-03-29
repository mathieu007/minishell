/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_pipes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/08/02 09:09:16 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*fork_first_child(t_cmd *pipe)
{
	pid_t		pid;
	t_process	*proc;
	t_cmd		*cmd;

	cmd = pipe->child;
	pipe_cmd(pipe);
	proc = get_process();
	if (build_cmd(cmd) == -1)
		return (close_prev_pipes(pipe), pipe->next);
	setup_child_signal_handlers(cmd);
	pid = ft_fork();
	if (pid == 0)
	{
		dup2(pipe->pipe->fd_out, STDOUT_FILENO);
		close_pipes(pipe->pipe);
		if (proc->errnum > 0)
			free_all_and_exit(proc->errnum);
		proc->errnum = dispatch_command(cmd, true);
		free_all_and_exit(proc->errnum);
	}
	reset_signal_handlers();
	pipe->pid = pid;
	return (pipe->next);
}

t_cmd	*fork_last_child(t_cmd *pipe)
{
	pid_t		pid;
	t_process	*proc;
	t_cmd		*cmd;

	cmd = pipe->child;
	proc = get_process();
	if (build_cmd(cmd) == -1)
		return (close_prev_pipes(pipe), pipe->next);
	setup_child_signal_handlers(cmd);
	pid = ft_fork();
	if (pid == 0)
	{
		dup2(pipe->prev->pipe->fd_in, STDIN_FILENO);
		close_prev_pipes(pipe);
		if (proc->errnum > 0)
			free_all_and_exit(proc->errnum);
		proc->errnum = dispatch_command(cmd, true);
		free_all_and_exit(proc->errnum);
	}
	reset_signal_handlers();
	pipe->pid = pid;
	close_prev_pipes(pipe);
	return (pipe);
}

void	dup_close_middle_pipes(t_cmd *cmd, t_cmd *pipe)
{
	dup2(pipe->prev->pipe->fd_in, STDIN_FILENO);
	if (cmd->has_redirection)
	{
		close_prev_pipes(pipe);
		close_pipes(pipe->pipe);
	}
	else
	{
		dup2(pipe->pipe->fd_out, STDOUT_FILENO);
		close_prev_pipes(pipe);
		close_pipes(pipe->pipe);
	}
}

t_cmd	*fork_middle_child(t_cmd *pipe)
{
	pid_t		pid;
	t_process	*proc;
	t_cmd		*cmd;

	cmd = pipe->child;
	pipe_cmd(pipe);
	proc = get_process();
	if (build_cmd(cmd) == -1)
		return (close_prev_pipes(pipe), pipe->next);
	setup_child_signal_handlers(cmd);
	pid = ft_fork();
	if (pid == 0)
	{
		dup_close_middle_pipes(cmd, pipe);
		if (proc->errnum > 0)
			free_all_and_exit(proc->errnum);
		proc->errnum = dispatch_command(cmd, true);
		free_all_and_exit(proc->errnum);
	}
	reset_signal_handlers();
	pipe->pid = pid;
	close_prev_pipes(pipe);
	return (pipe->next);
}

int32_t	exec_pipes_cmds(t_cmd *pipes)
{
	t_cmd		*start;
	t_process	*proc;

	proc = get_process();
	start = pipes;
	pipes = fork_first_child(pipes);
	while (pipes && pipes->next)
		pipes = fork_middle_child(pipes);
	if (pipes)
		pipes = fork_last_child(pipes);
	wait_childs(start);
	proc->execution = EXEC_END;
	return (proc->errnum);
}
