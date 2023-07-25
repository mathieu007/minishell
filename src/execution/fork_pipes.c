/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_pipes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/07/25 16:28:29 by mroy             ###   ########.fr       */
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
	proc->errnum = build_cmd(cmd);
	if (proc->errnum > 0 || proc->errnum == -1)
		return (pipe->next);
	setup_child_signal_handlers(cmd);
	pid = ft_fork();
	if (pid == 0)
	{
		dup2(pipe->pipe->fd_out, STDOUT_FILENO);
		close(pipe->pipe->fd_out);
		close(pipe->pipe->fd_in);
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
	proc->errnum = build_cmd(cmd);
	if (proc->errnum > 0 || proc->errnum == -1)
		return (pipe->next);
	setup_child_signal_handlers(cmd);
	pid = ft_fork();
	if (pid == 0)
	{
		dup2(pipe->prev->pipe->fd_in, STDIN_FILENO);
		close_prev_pipes(pipe);
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
		if (pipe->prev->pipe->fd_in > 2)
			close(pipe->prev->pipe->fd_in);
		if (pipe->prev->pipe->fd_out > 2)
			close(pipe->prev->pipe->fd_out);
		close_pipes(pipe->pipe);
	}
	else
	{
		dup2(pipe->pipe->fd_out, STDOUT_FILENO);
		close_prev_pipes(pipe);
		if (pipe->pipe->fd_in > 2)
			close(pipe->pipe->fd_in);
		if (pipe->pipe->fd_out > 2)
			close(pipe->pipe->fd_out);
	}
}

t_cmd	*fork_middle_child(t_cmd *pipe)
{
	pid_t		pid;
	t_process	*proc;
	t_cmd		*cmd;

	cmd = pipe->child;
	pipe_cmd(pipe);
	ft_printf("pipe in: %d\n", pipe->pipe->fd_in);
	ft_printf("pipe out: %d\n", pipe->pipe->fd_out);
	proc = get_process();
	proc->errnum = build_cmd(cmd);
	if (proc->errnum > 0 || proc->errnum == -1)
		return (pipe->next);
	setup_child_signal_handlers(cmd);
	pid = ft_fork();
	if (pid == 0)
	{
		dup_close_middle_pipes(cmd, pipe);
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
	return (proc->errnum);
}
